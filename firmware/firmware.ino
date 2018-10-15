#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

// You must create this file yourself, see README.md
#include "secrets.h"

// Configuration
#define PRECISE true      // Change to true if you need to define exact light positions
#define OLED_ENABLE false // Disable if not using an 128x32 OLED
#define BOARD_TYPE 1      // heltec == 0, hiletgo == 1
#define MAX_BRIGHTNESS 255

// Pin definitions
#if (BOARD_TYPE == 0)
#define NEOPIXEL_PIN 12     // GPIO12, D6
#define BUTTON_PIN_ONE 16   // GPIO16, D0
#define BUTTON_PIN_TWO 14   // GPIO14, SCL
#define BUTTON_PIN_THREE 13 // GPIO13, D7
#define BUTTON_PIN_FOUR 0   // GPIO15, D3
#else if (BOARD_TYPE == 1)
#define NEOPIXEL_PIN 2     // GPIO2, D4
#define BUTTON_PIN_ONE 16  // GPIO16, D0
#define BUTTON_PIN_TWO 4   // GPIO4, D2
#define BUTTON_PIN_THREE 0 // GPIO0, D3
#define BUTTON_PIN_FOUR 2  // GPIO2, D4
#endif

#define DISPLAY_MESSAGE_MODE 0
#define RAINBOW_MODE 1
#define SPOOKY_MODE 2
#define UNIBEAM_MODE 3

const char *HOST = "strtw.herokuapp.com"; // Or where-ever you deployed your API
const int HTTPS_PORT = 443;
const int8_t NUM_PIXELS = 100;
String message;
const int ERROR_MESSAGE_SIZE = 12;
// const int ERROR_MESSAGE[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
const int ERROR_MESSAGE[15] = {7, 0, 15, 15, 24, 7, 0, 11, 11, 14, 22, 4, 4, 13}; // "happyhalloween"
// const int ERROR_MESSAGE[12] = {13, 14, 19, 2, 14, 13, 13, 4, 2, 19, 4, 3}; // "notconnected"

int mode = DISPLAY_MESSAGE_MODE;
int colors[NUM_PIXELS];

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#if (OLED_ENABLE)
#include "oled.h"
#endif

#include "utils.h";

#include "effects.h";

#if (PRECISE)
#include "lightLetterPrecise.h";
#else
#include "lightLetter.h";
#endif

void setup() {
  pinMode(BUTTON_PIN_ONE, INPUT);
  pinMode(BUTTON_PIN_TWO, INPUT);
  pinMode(BUTTON_PIN_THREE, INPUT);
  pinMode(BUTTON_PIN_FOUR, INPUT);

  initColors();

  strip.begin();
  strip.show();

  Serial.begin(9600);

#if (OLED_ENABLE)
  resetOLED();
  display.clearDisplay();
  setText("Connecting to " + String(SSID), 0, 0);
  display.display();
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
    case SPOOKY_MODE:
      spooky();
      break;
    case UNIBEAM_MODE:
      unibeam();
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
        resetOLED();
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
          int8_t letterPosition = value.as<int8_t>();

          lightLetter(letterPosition);
        }
      }
    }

    client.stop();
  } else {
    Serial.println("connection failed!]");
    client.stop();

    errorMessages();
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
  setText("Not connected", 0, 8);
  setText("Mode: " + String(mode), 0, 16);
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
