#include <iostream>
#include "logger.h"
#include "appCore.h"


int AppCore::delayMs = 0;
bool AppCore::isActive = false;

HHOOK AppCore::hMouseHook;
std::thread AppCore::mHookerThread;
std::thread AppCore::clickThread;
static constexpr std::chrono::duration<double> MinSleepDuration(0);

std::mutex AppCore::clickMtx;
std::condition_variable AppCore::clickRefresh;

bool AppCore::LButtonState = false;
bool AppCore::RButtonState = false;
bool AppCore::MButtonState = LButtonState;
std::chrono::system_clock::time_point AppCore::rButtonPressedStart;

void AppCore::setMButtonState(bool isDown) {
    MButtonState = false;
    mouse_event(isDown ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
}

BOOLEAN AppCore::nanosleep(LONGLONG ns) {
    HANDLE timer;
    LARGE_INTEGER li;

    if(!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;

    li.QuadPart = -ns;

    if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)){
        CloseHandle(timer);
        return FALSE;
    }

    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);

    return TRUE;
}

void AppCore::onLButtonChange(bool isDown) {
    if (delayMs == 0) {
        setMButtonState(LButtonState);
        return;
    }

    if (isDown) {
        if (!RButtonState) {
            setMButtonState(isDown);
        } 
        else {
            std::thread([](){
                // ! METHOD 1
                // std::chrono::system_clock::time_point now = std::chrono::high_resolution_clock::now();
                // std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - rButtonPressedStart);
                // int cDuration = duration.count();
                // if (cDuration < delayMs)
                //     std::this_thread::sleep_for(std::chrono::milliseconds(delayMs - cDuration));

                // ! METHOD 2
                // while ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - rButtonPressedStart)).count() < delayMs) {
                //     std::this_thread::sleep_for(MinSleepDuration);
                // }

                // ! METHOD 3
                std::chrono::system_clock::time_point now = std::chrono::high_resolution_clock::now();
                std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - rButtonPressedStart);
                int cDuration = duration.count();
                if (cDuration < delayMs)
                    nanosleep((delayMs - cDuration) * 10000);

                if (LButtonState != MButtonState)
                    setMButtonState(LButtonState);
            }).detach();
        }
    }
    else {
        setMButtonState(isDown);
    }
}

void AppCore::onRButtonChange(bool isDown) {
    if (delayMs == 0)
        return;

    if (isDown) {
        rButtonPressedStart = std::chrono::high_resolution_clock::now();
    } 
    else { // TODO: It's only for hold type shooting. Make variable for click type shooting.
        if (LButtonState != MButtonState)
            setMButtonState(LButtonState);
    }
}

void AppCore::clickWorker() {
    logger("AppCore::clickWorker", "Init");
    bool LButtonStateCache = LButtonState;
    bool RButtonStateCache = RButtonState;

    while (true)
    {
        std::unique_lock<std::mutex> locker(clickMtx);
        clickRefresh.wait(locker);

        if (LButtonState != LButtonStateCache) {
            LButtonStateCache = LButtonState;
            onLButtonChange(LButtonState);
        }
        
        if (RButtonState != RButtonStateCache) {
            RButtonStateCache = RButtonState;
            onRButtonChange(RButtonState);
        }
    }

    logger("AppCore::clickWorker", "Exiting...");
}

LRESULT WINAPI AppCore::mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (isActive) {
        if (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP) {
            LButtonState = (wParam == WM_LBUTTONDOWN);
            clickRefresh.notify_one();
        }
        else if (wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP) {
            RButtonState = (wParam == WM_RBUTTONDOWN);
            clickRefresh.notify_one();
        }
    }

    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

DWORD WINAPI AppCore::mouseHooker() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    logger("AppCore::mouseHooker", "Hook set");
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, hInstance, 0);

    MSG message;
    while (GetMessage(&message, 0, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    UnhookWindowsHookEx(hMouseHook);
    logger("AppCore::mouseHooker", "Hook unhooked");
    return 0;
}

BOOL WINAPI AppCore::CtrlHandler(DWORD fdwCtrlType) {
    Dispose();
    return false;
}

void AppCore::Init() {
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    clickThread = std::thread(clickWorker);
    mHookerThread = std::thread(mouseHooker);
}

void AppCore::Start() {
    logger("AppCore::Start", "Working");
    isActive = true;
}

void AppCore::Start(int delayMs) {
    SetDelay(delayMs);
    Start();
}

bool AppCore::IsRunning() {
    return isActive;
}

void AppCore::Suspend() {
    isActive = false;
    logger("AppCore::Suspend", "Work Suspend");
}

void AppCore::Dispose() {
    UnhookWindowsHookEx(hMouseHook);
    logger("AppCore::Dispose", "hMouseHook unhooked");
    mHookerThread.~thread();
    logger("AppCore::Dispose", "mHookerThread disposed");
    clickThread.~thread();
    logger("AppCore::Dispose", "clickThread disposed");
}

void AppCore::SetDelay(int delayMs) {
    AppCore::delayMs = delayMs;
    logger("AppCore::SetDelay", std::string("Delay set: ") + std::to_string(delayMs));
}

int AppCore::GetDelay() {
    return delayMs;
}