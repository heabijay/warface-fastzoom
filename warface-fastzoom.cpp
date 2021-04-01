#include <chrono>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <windows.h>
#include "logger.cpp"

using namespace std;

const int delayMs = 50;

HHOOK hMouseHook;
thread mHookerThread;
thread clickThread;

bool isActive = false;

mutex clickMtx;
condition_variable clickRefresh;
bool LButtonState = false;
bool RButtonState = false;
bool MButtonState = LButtonState;
chrono::system_clock::time_point rButtonPressedStart;

void setMButtonState(bool isDown) {
    MButtonState = false;
    mouse_event(isDown ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
}

void onLButtonChange(bool isDown) {
    if (!isActive) {
        setMButtonState(LButtonState);
        return;
    }

    if (isDown) {
        if (!RButtonState) {
            setMButtonState(isDown);
        } 
        else {
            thread([](){
                chrono::system_clock::time_point now = chrono::system_clock::now();
                chrono::milliseconds duration = chrono::duration_cast<chrono::milliseconds>(now - rButtonPressedStart);
                int cDuration = duration.count();
                if (cDuration < delayMs)
                    this_thread::sleep_for(chrono::milliseconds(delayMs - cDuration));

                if (LButtonState != MButtonState)
                    setMButtonState(LButtonState);
            }).detach();
        }
    }
    else {
        setMButtonState(isDown);
    }
}

void onRButtonChange(bool isDown) {
    if (!isActive)
        return;

    if (isDown) {
        rButtonPressedStart = chrono::system_clock::now();
    } 
    else {
        if (LButtonState != MButtonState)
            setMButtonState(LButtonState);
    }
}

void clickWorker()
{
    logger("clickWorker", "Init");
    bool LButtonStateCache = LButtonState;
    bool RButtonStateCache = RButtonState;

    while (true)
    {
        unique_lock<mutex> locker(clickMtx);
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

    logger("clickWorker", "Exiting...");
}

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP) {
        LButtonState = (wParam == WM_LBUTTONDOWN);
        clickRefresh.notify_one();
    }
    else if (wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP) {
        RButtonState = (wParam == WM_RBUTTONDOWN);
        clickRefresh.notify_one();
    }

    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

DWORD WINAPI mouseHooker() 
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    logger("mouseHooker", "Hook set");
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, hInstance, 0);

    MSG message;
    while (GetMessage(&message, 0, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    UnhookWindowsHookEx(hMouseHook);
    logger("mouseHooker", "Hook unhooked");
    return 0;
}

void keyboardChecker()
{
    logger("keyboardChecker", "Init");

    while (true)
    {
        if ((GetAsyncKeyState(VK_NUMPAD0) & 0x8000) && isActive) {
            isActive = false;
            logger("keyboardChecker", "isActive = false");
        }

        if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000) && !isActive) {
            isActive = true;
            logger("keyboardChecker", "isActive = true");
        }

        if (GetAsyncKeyState(VK_END) & 0x8000) {
            logger("keyboardChecker", "Exit initialized");
            break;
        }

        Sleep(10);
    }

    logger("keyboardChecker", "Exiting...");
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    UnhookWindowsHookEx(hMouseHook);
    logger("CtrlHandler", "hMouseHook unhooked");
    mHookerThread.~thread();
    logger("CtrlHandler", "mHookerThread disposed");
    clickThread.~thread();
    logger("CtrlHandler", "clickThread disposed");

    return false;
}

int main(int argc, char **argv)
{
    cout << "| Warface FastZoom Helper | " << endl;
    cout << endl;
    cout << "Developed by heabijay (t.me/heabijay)" << endl;
    cout << "Build Date: 30.03.2021" << endl;
    cout << endl;
    cout << "Instructions: " << endl;
    cout << "1. Change shoot button in game to 'Mouse Button 3' (Mouse Wheel click)" << endl;
    cout << endl;

    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    clickThread = thread(clickWorker);
    mHookerThread = thread(mouseHooker);
    keyboardChecker();
}   