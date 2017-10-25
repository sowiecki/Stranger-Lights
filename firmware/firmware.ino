#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define PIN 2 // Actually 04 on Makerfocus D1 Mini
#define GREEN_BUTTON_PIN 14
#define RED_BUTTON_PIN 12

int greenButtonState = 0;
int redButtonState = 0;

bool shouldDisplayMessage = false;
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
  pinMode(GREEN_BUTTON_PIN, INPUT);
  pinMode(RED_BUTTON_PIN, INPUT);

  initColors();

  strip.begin();
  strip.show();

  Serial.begin(9600);
  Serial.printf("Connecting to %s ", SSID);
  WiFi.begin(SSID, PASSWORD);

  Serial.println(" connected");
}

void loop() {
  checkButtonState();

  if (WiFi.status() != WL_CONNECTED && shouldDisplayMessage) {
    colorWipe();
    lightErrorMessage();
    Serial.print(".");
  } else if (shouldDisplayMessage) {
    colorWipe();
    displayMessage();
  } else {
    lightAll();
  }
}

void displayMessage() {
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", HOST);
  if (client.connect(HOST, 80)) {
    client.print(String("GET /" + PATH) + " HTTP/1.1\r\n" + "Host: " + HOST +
                 "\r\n" + "Connection: close\r\n" + "\r\n");

    while (client.connected() && shouldDisplayMessage) {
      if (client.available()) {
        message = client.readStringUntil('\n');
        JsonArray &pixelsArray = jsonBuffer.parseArray(message);

        for (auto value : pixelsArray) {
          Serial.println(value.as<char *>());
          const int letterPosition = value.as<int>();
          lightLetter(letterPosition);
        }
      }
    }

    client.stop();
  } else {
    Serial.println("connection failed!]");
    client.stop();
  }
  wait(300);
}

void lightLetter(int pixel) {
  checkButtonState();

  if (pixel >= 8 && pixel <= 16) {
    pixelToLight = pixel + 1;
  } else {
    pixelToLight = 25 - pixel;
  }

  strip.setPixelColor(pixelToLight, 0);
  strip.show();
  wait(100);
  strip.setPixelColor(pixelToLight, colors[pixelToLight]);
  strip.show();
  wait(550);
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
    lightLetter(ERROR_MESSAGE[i]);
  }
}

void colorWipe() {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
  }
}

void checkButtonState() {
  greenButtonState = digitalRead(GREEN_BUTTON_PIN);
  redButtonState = digitalRead(RED_BUTTON_PIN);

  if (greenButtonState == LOW) {
    shouldDisplayMessage = true;
  } else if (redButtonState == HIGH) {
    shouldDisplayMessage = false;
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

void wait(int ms) {
  if (shouldDisplayMessage) {
    delay(ms);
  }
}
