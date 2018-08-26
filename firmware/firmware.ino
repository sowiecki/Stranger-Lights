#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define PIN 2 // Actually 04 on Makerfocus D1 Mini

const bool PRECISE =
    false; // Change to true if you need to define exact light positions
const char *SSID = "";
const char *PASSWORD = "";
const char *HOST = "strtw.herokuapp.com"; // Or where-ever you deployed your API
String PATH = "nasezero";
const int HTTPS_PORT = 443;
const int8_t NUM_PIXELS = 26;
int colors[NUM_PIXELS];
int8_t pixelToLight;
String message;
DynamicJsonBuffer jsonBuffer;
const int ERROR_MESSAGE_SIZE = 12;
const int ERROR_MESSAGE[12] = {13, 14, 19, 2,  14, 13,
                               13, 4,  2,  19, 4,  3}; // "notconnected"

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
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
    colorWipe();
    lightErrorMessage();
    Serial.print(".");
    colorWipe();
    displayMessage();
  }
}

void displayMessage() {
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", HOST);
  if (client.connect(HOST, 80)) {
    client.print(String("GET /" + PATH) + " HTTP/1.1\r\n" + "Host: " + HOST +
                 "\r\n" + "Connection: close\r\n" + "\r\n");

    while (client.connected()) {
      if (client.available()) {
        message = client.readStringUntil('\n');
        JsonArray &pixelsArray = jsonBuffer.parseArray(message);

        for (auto value : pixelsArray) {
          Serial.println(value.as<char *>());
          const int letterPosition = value.as<int>();
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
  delay(1050);
  strip.setPixelColor(pixelToLight, 0);
  strip.show();
}

// Change each letter to the actual index of that letter's light position
void lightPreciseLetter(int pixel) {
  Serial.println(pixel);
  if (pixel == 0) {
    pixelToLight = 62; // a
  } else if (pixel == 1) {
    pixelToLight = 60; // b
  } else if (pixel == 2) {
    pixelToLight = 59; // c
  } else if (pixel == 3) {
    pixelToLight = 56; // d
  } else if (pixel == 4) {
    pixelToLight = 54; // e
  } else if (pixel == 5) {
    pixelToLight = 51; // f
  } else if (pixel == 6) {
    pixelToLight = 49; // g
  } else if (pixel == 7) {
    pixelToLight = 47; // h
  } else if (pixel == 8) {
    pixelToLight = 24; // i
  } else if (pixel == 9) {
    pixelToLight = 26; // j
  } else if (pixel == 10) {
    pixelToLight = 28; // k
  } else if (pixel == 11) {
    pixelToLight = 30; // l
  } else if (pixel == 12) {
    pixelToLight = 32; // m
  } else if (pixel == 13) {
    pixelToLight = 35; // n
  } else if (pixel == 14) {
    pixelToLight = 38; // o
  } else if (pixel == 15) {
    pixelToLight = 40; // p
  } else if (pixel == 16) {
    pixelToLight = 42; // q
  } else if (pixel == 17) {
    pixelToLight = 19; // r
  } else if (pixel == 18) {
    pixelToLight = 17; // s
  } else if (pixel == 19) {
    pixelToLight = 16; // t
  } else if (pixel == 20) {
    pixelToLight = 12; // u
  } else if (pixel == 21) {
    pixelToLight = 11; // v
  } else if (pixel == 22) {
    pixelToLight = 8; // w
  } else if (pixel == 23) {
    pixelToLight = 5; // x
  } else if (pixel == 24) {
    pixelToLight = 3; // y
  } else if (pixel == 25) {
    pixelToLight = 1; // z
  }
}

void lightAll() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, colors[i]);
    strip.show();
  }
}

void lightErrorMessage() {
  for (int i = 0; i < ERROR_MESSAGE_SIZE; i++) {
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
