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

        if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000) && (!AppCore::IsRunning() || AppCore::GetDelay() != 50)) {
            AppCore::Start(50);
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