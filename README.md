## Bluetooth LE Zoom Muter

A physical button to mute Zoom on iPad OS and Windows. Also sends keys through the Serial Monitor.

Based on [twstokes/labs/zoommuter](https://github.com/twstokes/labs/tree/master/zoommuter). Check out
[the website](https://www.tannr.com/2020/04/10/ipad-zoom-mute-button/). Keyboard functionality based
on the [Bluefruit example 'hidkeyboard'](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51/blob/master/examples/hidkeyboard/hidkeyboard.ino).

NOTE: The mute button requires the iPad to map Option --> Command key.

This works for both iOS and Windows. The iOS mute hotkey for Zoom is Command+Shift+A. The Windows
mute hotkey for Zoom is Alt+A. This presses one after the other. The one for the wrong platform is
simply ignored by Zoom.

To send keystrokes, open the Serial Monitor while the device is plugged in to your computer, and type
the keys you want to send into the serial monitor.

### Hardware
- [Adafruit BLE ATmega32u4](https://www.adafruit.com/product/2661) or [Adafruit Feather 32u4 BLE](https://www.adafruit.com/product/2829). I'm using the Feather.
- iPad with Zoom installed
- A button
- 10k ohm resistor to prevent floating input

### How it works
1. Connect up a button to the MCU. This code uses pin A0 for digital input, defined by `BUTTON_PIN`. [Here's an example.](https://www.arduino.cc/en/tutorial/button)
1. Pair the button using Bluetooth settings
1. Tell iPad OS to remap Option to Command (General -> Hardware Keyboard -> Modifier Keys)
1. Load up Zoom
1. Hit the button!

### Why it works

Since the BLE device acts as a keyboard, it's simply hitting shift + alt + a. [See keycodes here.](https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/ble-services#at-plus-blekeyboardcode-14-25)

### Building

1. Install [PlatformIO](https://platformio.org/)
1. Connect the MCU and run `pio run -t upload`


### Sending keystrokes

`pio device monitor`, then type some stuff. Type `\r` for return.

### Gotchas

- Monitor the serial output to make sure things are OK
- If Bluetooth starts acting flaky, reset the microcontroller using the [Adafruit reset example](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51/tree/master/examples/factoryreset).

