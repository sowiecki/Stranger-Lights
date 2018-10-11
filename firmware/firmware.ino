#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "secrets.h"

#define PRECISE true     // Change to true if you need to define exact light positions
#define OLED_ENABLE true // Disable if not using an 128x32 OLED

// #define NEOPIXEL_PIN 2      // GPIO2, D4 on HiLetgo
#define NEOPIXEL_PIN 13     // GPIO13, D7 on Heltec
#define BUTTON_PIN_GREEN 16 // GPIO16, D0
#define BUTTON_PIN_RED 4    // GPIO0, D2
#define DISPLAY_MESSAGE_MODE 0
#define RAINBOW_MODE 1

const char *HOST = "strtw.herokuapp.com"; // Or where-ever you deployed your API
const int HTTPS_PORT = 443;
const int8_t NUM_PIXELS = 100;
String message;
const int ERROR_MESSAGE_SIZE = 12;
const int ERROR_MESSAGE[15] = {7, 0, 15, 15, 24, 7, 0, 11, 11, 14, 22, 4, 4, 13}; // "happyhalloween"
// const int ERROR_MESSAGE[12] = {13, 14, 19, 2, 14, 13, 13, 4, 2, 19, 4, 3}; // "notconnected"

int mode = DISPLAY_MESSAGE_MODE;
int colors[NUM_PIXELS];
int8_t pixelToLight;

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#include "utils.h";

#if (OLED_ENABLE)
#include "oled.h"
#endif

#if (PRECISE)
#include "lightLetter.h";
#else
#include "lightLetterPrecise.h";
#endif

void setup() {
  pinMode(BUTTON_PIN_GREEN, INPUT);
  pinMode(BUTTON_PIN_RED, INPUT);

  initColors();

  strip.begin();
  strip.show();

  Serial.begin(9600);

#if (OLED_ENABLE)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  setText("Connecting to " + String(SSID), 0, 0);
#endif
  Serial.printf("Connecting to %s ", SSID);
  WiFi.begin(SSID, PASSWORD);
  Serial.println(" connected");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    errorMessages();
    Serial.print(".");
  } else {
    logDeviceData();

    switch (mode) {
    case DISPLAY_MESSAGE_MODE:
      colorWipe();
      displayMessage();
      break;
    case RAINBOW_MODE:
      rainbow(50);
      break;
    }
  }
}

void displayMessage() {
  WiFiClient client;
  DynamicJsonBuffer jsonBuffer;

  Serial.printf("\nConnecting to %s ... ", HOST);

  if (mode == DISPLAY_MESSAGE_MODE && client.connect(HOST, 80)) {
    client.print(String("GET /" + PATH) + " HTTP/1.1\r\n" + "Host: " + HOST +
                 "\r\n" + "Connection: close\r\n" + "\r\n");

    while (client.connected() && WiFi.status() == WL_CONNECTED) {
      if (client.available()) {
        message = client.readStringUntil('\n');
        JsonObject &messageData = jsonBuffer.parseObject(message);
        JsonArray &pixelsArray = messageData["indexes"];
        String rawText = messageData["rawText"];

        Serial.println(rawText);
#if (OLED_ENABLE)
        display.clearDisplay();
        setText("@" + PATH, 0, 0);
        setText(rawText, 0, 8);
        display.display();
        display.startscrollleft(0x00, 0x10);
#endif

        for (auto value : pixelsArray) {
          checkButtonStates();
          if (mode != DISPLAY_MESSAGE_MODE)
            break;
          int letterPosition = value.as<int>();

          lightLetter(letterPosition);
        }
      }
    }

    client.stop();
  } else {
    Serial.println("connection failed!]");
    client.stop();
  }

  delay(300);
}

void lightAll() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, colors[i]);
    strip.show();
  }
}

void errorMessages() {
#if (OLED_ENABLE)
  display.clearDisplay();
  setText("@" + PATH, 0, 0);
  setText("Error, not connected!", 0, 8);
  display.display();
  display.startscrollleft(0x00, 0x10);
#endif

  for (int i = 0; i < ERROR_MESSAGE_SIZE; i++) {
    if (WiFi.status() == WL_CONNECTED || mode != DISPLAY_MESSAGE_MODE)
      break;
    lightLetter(ERROR_MESSAGE[i]);
  }
}

void colorWipe() {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
  }
}

void initColors() {
  for (int i = 0; i < strip.numPixels(); i++) {
    int rgb = i % 5;
    switch (rgb) {
    case 0:
      colors[i] = strip.Color(255, 0, 0); // green
      break;
    case 1:
      colors[i] = strip.Color(0, 255, 0); // red
      break;
    case 2:
      colors[i] = strip.Color(0, 0, 255); // blue
      break;
    case 3:
      colors[i] = strip.Color(255, 0, 255); // magenta
      break;
    case 4:
      colors[i] = strip.Color(255, 255, 0); // yellow
      break;
    }
  }
}

void logDeviceData() {
  Serial.print("Mode: ");
  Serial.println(mode);

  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("Device MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("Connecting to: ");
  Serial.println(HOST);

  Serial.print("Memory heap: ");
  Serial.println(ESP.getFreeHeap());
}

// From Adafruit NeoPixel example
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    if (mode != RAINBOW_MODE)
      break;
    for (i = 0; i < strip.numPixels(); i++) {
      checkButtonStates();
      if (mode != RAINBOW_MODE)
        break;
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// From Adafruit NeoPixel example
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
