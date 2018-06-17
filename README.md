# Stranger Lights

![demo](./demo.gif)

# Hardware
* [MakerFocus ESP8266](https://smile.amazon.com/gp/product/B076JDVRLP)
* A strand/sequence of (at least) 26 WS2812 LEDs (search for "addressable Christmas lights", make certain they are WS2812 compatible)
* Wallpaper and black paint

# Setup
Hang a strang of addressable LEDs, starting at the letter Z snaking up through I to Q, then up to H through A.
Assuming a strange of exactly 26 pixels, see the pixel the [Pixel Map](./PIXELMAP.md) for which pixel index corresponds to which letter.

### Firmware
Follow the [board installation instructions](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/blob/master/InstallGuide). In the Arduino IDE, set the board type to `WIFI_Kit_8`.

In [firmware/firmware.ino](./firmware/firmware.ino), set `*SSID` `*PASSWORD` to the SSID and password of your WiFi network.

If your light strand doesn't correspond exactly to each letter, e.g. you have a strand of 50 lights with some being unused between each letter, set `PRECISE` to `false` and change the values in the method `lightPreciseLetter` to match your lights' positions.

### Twitter API
Deploy this application to Heroku, or feel free to use `strtw.herokuapp.com`. I'll keep it freely available as long as the rate limit doesn't get hit.
