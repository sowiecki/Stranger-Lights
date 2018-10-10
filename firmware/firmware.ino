#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "secrets.h"

#define NEOPIXEL_PIN 2      // GPIO2, D4
#define BUTTON_PIN_GREEN 16 // GPIO16, D0
#define BUTTON_PIN_RED 4    // GPIO0, D2
#define DISPLAY_MESSAGE_MODE 0
#define RAINBOW_MODE 1

const bool PRECISE = true;                // Change to true if you need to define exact light positions
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

void setup() {
  pinMode(BUTTON_PIN_GREEN, INPUT);
  pinMode(BUTTON_PIN_RED, INPUT);

  initColors();

  strip.begin();
  strip.show();

  Serial.begin(9600);

  Serial.printf("Connecting to %s ", SSID);
  WiFi.begin(SSID, PASSWORD);
  Serial.println(" connected");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    lightErrorMessage();
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
        JsonArray &pixelsArray = jsonBuffer.parseArray(messageData["indexes"]);
        String rawText = messageData["rawText"];

        Serial.println(rawText);

        for (auto value : pixelsArray) {
          checkButtonStates();
          if (mode != DISPLAY_MESSAGE_MODE)
            break;
          int letterPosition = value.as<int>();

          if (!PRECISE) {
            lightLetter(letterPosition);
          } else {
            lightPreciseLetter(letterPosition);
          }
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

void checkButtonStates() {
  if (digitalRead(BUTTON_PIN_GREEN) == LOW) {
    mode = DISPLAY_MESSAGE_MODE;
  } else if (digitalRead(BUTTON_PIN_RED) == LOW) {
    mode = RAINBOW_MODE;
  }
}

void lightLetter(int pixel) {
  if (pixel >= 8 && pixel <= 16) {
    pixelToLight = pixel + 1;
  } else {
    pixelToLight = 25 - pixel;
  }

  strip.setPixelColor(pixelToLight, 0);
  strip.show();
  delay(200);
  strip.setPixelColor(pixelToLight, colors[pixelToLight]);
  strip.show();

  lazyDelay();

  strip.setPixelColor(pixelToLight, 0);
  strip.show();
}

// Change each letter to the actual index of that letter's light position
void lightPreciseLetter(int pixel) {
  if (pixel == 0) {
    pixelToLight = 61; // a
  } else if (pixel == 1) {
    pixelToLight = 59; // b
  } else if (pixel == 2) {
    pixelToLight = 57; // c
  } else if (pixel == 3) {
    pixelToLight = 55; // d
  } else if (pixel == 4) {
    pixelToLight = 54; // e
  } else if (pixel == 5) {
    pixelToLight = 52; // f
  } else if (pixel == 6) {
    pixelToLight = 51; // g
  } else if (pixel == 7) {
    pixelToLight = 50; // h
  } else if (pixel == 8) {
    pixelToLight = 26; // i
  } else if (pixel == 9) {
    pixelToLight = 29; // j
  } else if (pixel == 10) {
    pixelToLight = 30; // k
  } else if (pixel == 11) {
    pixelToLight = 32; // l
  } else if (pixel == 12) {
    pixelToLight = 34; // m
  } else if (pixel == 13) {
    pixelToLight = 36; // n
  } else if (pixel == 14) {
    pixelToLight = 38; // o
  } else if (pixel == 15) {
    pixelToLight = 39; // p
  } else if (pixel == 16) {
    pixelToLight = 41; // q
  } else if (pixel == 17) {
    pixelToLight = 19; // r
  } else if (pixel == 18) {
    pixelToLight = 17; // s
  } else if (pixel == 19) {
    pixelToLight = 15; // t
  } else if (pixel == 20) {
    pixelToLight = 14; // u
  } else if (pixel == 21) {
    pixelToLight = 12; // v
  } else if (pixel == 22) {
    pixelToLight = 11; // w
  } else if (pixel == 23) {
    pixelToLight = 9; // x
  } else if (pixel == 24) {
    pixelToLight = 7; // y
  } else if (pixel == 25) {
    pixelToLight = 5; // z
  }

  Serial.println(pixelToLight);

  strip.setPixelColor(pixelToLight, 0);
  strip.show();
  delay(200);
  strip.setPixelColor(pixelToLight, colors[pixelToLight]);
  strip.show();

  lazyDelay();

  strip.setPixelColor(pixelToLight, 0);
  strip.show();
}

void lightAll() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, colors[i]);
    strip.show();
  }
}

void lightErrorMessage() {
  for (int i = 0; i < ERROR_MESSAGE_SIZE; i++) {
    if (WiFi.status() == WL_CONNECTED || mode != DISPLAY_MESSAGE_MODE)
      break;
    if (!PRECISE) {
      lightLetter(ERROR_MESSAGE[i]);
    } else {
      lightPreciseLetter(ERROR_MESSAGE[i]);
    }
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

void lazyDelay() {
  // Lazy way of checking button states while keeping paused for 1050ms
  delay(300);
  checkButtonStates();
  delay(250);
  checkButtonStates();
  delay(250);
  checkButtonStates();
  delay(250);
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
