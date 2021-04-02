#include <chrono>
#include <condition_variable>
#include <thread>
#include <windows.h>

#ifndef AppCore_H
#define AppCore_H

class AppCore
{
private:
    AppCore() { }

    static HHOOK hMouseHook;
    static std::thread mHookerThread;
    static std::thread clickThread;

    static int delayMs;
    static bool isActive;

    static std::mutex clickMtx;
    static std::condition_variable clickRefresh;
    static bool LButtonState;
    static bool RButtonState;
    static bool MButtonState;
    static std::chrono::system_clock::time_point rButtonPressedStart;

    static void setMButtonState(bool isDown);
    static void clickWorker();
    static LRESULT mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static DWORD mouseHooker();
    static void keyboardChecker();
    static BOOL CtrlHandler(DWORD fdwCtrlType);
protected:
    static void onLButtonChange(bool isDown);
    static void onRButtonChange(bool isDown);
public:
    static void Init();
    static void Start();
    static void Start(int delayMs);
    static bool IsRunning();
    static void Suspend();
    static void Dispose();

    static int GetDelay();
    static void SetDelay(int delayMs);
};

#endif