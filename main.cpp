#include <iostream>
#include "appCore.h"
#include "logger.h"
#include "appSettings.h"

using namespace std;

void keyboardChecker()
{
    logger("keyboardChecker", "Init");

    while (true)
    {
        if ((GetAsyncKeyState(VK_DECIMAL) & 0x8000) && AppCore::IsRunning()) {
            AppCore::Suspend();
        }

        if ((GetAsyncKeyState(VK_NUMPAD0) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 0)) {
            AppCore::Start(0);
        }

        if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 80)) {
            AppCore::Start(80);
        }

        if ((GetAsyncKeyState(VK_NUMPAD2) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 120)) {
            AppCore::Start(120);
        }

        if ((GetAsyncKeyState(VK_NUMPAD3) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 130)) {
            AppCore::Start(130);
        }

        if ((GetAsyncKeyState(VK_NUMPAD4) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 140)) {
            AppCore::Start(140);
        }

        if ((GetAsyncKeyState(VK_NUMPAD5) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 150)) {
            AppCore::Start(150);
        }

        if ((GetAsyncKeyState(VK_NUMPAD6) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 170)) {
            AppCore::Start(170);
        }

        if ((GetAsyncKeyState(VK_NUMPAD7) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 180)) {
            AppCore::Start(180);
        }

        if ((GetAsyncKeyState(VK_NUMPAD8) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 190)) {
            AppCore::Start(190);
        }

        if ((GetAsyncKeyState(VK_NUMPAD9) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 200)) {
            AppCore::Start(200);
        }

        if (GetAsyncKeyState(VK_END) & 0x8000) {
            break;
        }

        Sleep(10);
    }

    logger("keyboardChecker", "Exiting...");
}


int main(int argc, char **argv)
{
    cout << "| Warface FastZoom Helper | " << endl;
    cout << endl;
    cout << "Developed by heabijay (t.me/heabijay)" << endl;
    cout << endl;
    cout << "Instructions: " << endl;
    cout << "1. Change shoot button in game to 'Mouse Button 3' (Mouse Wheel click)" << endl;
    cout << endl;

    AppCore::Init();
    keyboardChecker();
    AppCore::Dispose();
}   