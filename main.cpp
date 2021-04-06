#include <iostream>
#include <fstream>
#include "logger.h"
#include "appCore.h"
#include "appSettings.h"

using namespace std;

const char* filename = "settings.ini";
struct AppSettings::Settings settings;

void keyboardChecker()
{
    logger("keyboardChecker", "Init");

    int i;
    while (true)
    {
        if ((GetAsyncKeyState(VK_DECIMAL) & 0x8000) && AppCore::IsRunning()) {
            AppCore::Suspend();
            logger("keyboardChecker", "Disabled Profile");
        }

        for (i = 0; i < 10; i++) {
            if (GetAsyncKeyState(VK_NUMPAD0 + i) & 0x8000) {
                if (settings.binds[i].name.size() > 0) {
                    AppCore::Start(settings.binds[i].delay);
                    logger(
                        "keyboardChecker", 
                        string("Activated Profile #") + 
                            string(to_string(i)) + 
                            string(": ") + 
                            settings.binds[i].name + 
                            string(" (") + 
                            string(to_string(settings.binds[i].delay)) + 
                            string("ms)")
                        );
                    
                    while (GetAsyncKeyState(VK_NUMPAD0 + i) & 0x8000) { }
                }
            }
        }

        Sleep(10);
    }

    logger("keyboardChecker", "Exiting...");
}


int main(int argc, char **argv)
{
    std::ifstream f(filename);
    bool settingsSuccess = false;
    if (f.good()) {
        try {
            settings = AppSettings::Deserialize(f);
            cout << "Settings successfully loaded!" << endl;
            settingsSuccess = true;
        }
        catch (AppSettings::Exceptions::UnsupportedAppName) {
            cout << "Settings file `settings.ini` has unsupported app name. You could reset the default settings by force delete file." << endl;
        }
        catch (AppSettings::Exceptions::InvalidAppVersion) {
            cout << "Settings file `settings.ini` has unsupported app name. You could reset the default settings by force delete file." << endl;
        }
        catch (AppSettings::Exceptions::UnsupportedAppVersion) {
            cout << "Settings file `settings.ini` has higher app version than you have. Please, update app or you could reset the default settings by force delete file." << endl;
        }
        f.close();
    }
    else {
        f.close();
        settings = AppSettings::Settings::ProvideDefault();
        std::ofstream fout(filename);
        AppSettings::Serialize(fout, settings);
        fout.close();
        cout << "Settings created just now in file `settings.ini`. You could change default settings by editing it!" << endl;
        settingsSuccess = true;
    }

    if (!settingsSuccess) {
        system("pause");
        return 1;
    }

    cout << endl;
    cout << "| Warface FastZoom Helper | " << endl;
    cout << endl;
    cout << "Developed by heabijay (t.me/heabijay)" << endl;
    cout << endl;

    cout << "Instructions: " << endl;
    cout << "1. Change shoot button in game to 'Mouse Button 3' (Mouse Wheel click)" << endl;
    cout << "2. Press keys on numpad for change delays." << endl;
    cout << "3. Enjoy!" << endl;
    cout << endl;

    cout << "Settings:" << endl;
    cout << "  Bindings:" << endl;
    for (int i = 0; i < 10; i++) {
        if (settings.binds[i].name.size() > 0)
        cout << "    NUMPAD" << i << ": " << settings.binds[i].name << " (" << settings.binds[i].delay << "ms)" << endl;
    }
    cout << endl;

    AppCore::Init();
    keyboardChecker();
    AppCore::Dispose();
}   