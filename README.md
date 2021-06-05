# warface-fastzoom

This application clones your clicks to a different button with await delay while you fastzooms.

## Quick Guide

1. Open the Warface game and change in-game shoot to mouse button 3 (Middle Mouse Button / Mouse Wheel Click). 

    _**Note:** Now the app is adapted for in-game zoom using **hold** right mouse button. If you using a click instead of hold, you could have problems with shoot timings._

2. Open the app. In the first run, the application should create the `settings.ini` file with settings. There you could change the default settings. Settings will be applied in the next app launch.

3. Use hotkeys to configure delays.

4. Be happy!

## Hotkeys: 

* **Numpad Decimal** (VK_DECIMAL) to **disable** all callbacks on Middle Mouse Button.

* **Numpads 0-9** binds sets in `settings.ini`. By default:

  * **Numpad 0** — Sets delay 0ms (Disabled). It should be just callbacks to Middle Mouse Button but without delay.

  * **Numpad 1** — Sets delay 80ms (Steyr Scout).

  * **Numpad 2** — Sets delay 120ms (AT308).

  * **Numpad 3** — Sets delay 140ms (ОРСИС Т-5000).

  * **Numpad 4** — Sets delay 200ms (CheyTac M200).

## TODO

1. Function to use delays only on primary weapon (by monitoring 1-2-3-4-5 keys)

2. Make adaptation for click instead of hold right mouse button.

## Safety

The app doesn't inject into the game and doesn't interact with it. App just emulating mouse click. So it looks safe.

So, **use this program for your own risks**.