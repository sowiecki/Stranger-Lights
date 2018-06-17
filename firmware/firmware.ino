#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <U8g2lib.h>

//U8g2 Contructor
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);
// Alternative board version. Uncomment if above doesn't work.
// U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 2, /* reset=*/ 4);

u8g2_uint_t offset;     // current offset for the scrolling text
u8g2_uint_t width;      // pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
const char *text = "STRANGER THINGS "; // scroll this text from right to left

#define PIN 2 // Actually 04 on Makerfocus D1 Mini
#define GREEN_BUTTON_PIN 14
#define RED_BUTTON_PIN 12

int greenButtonState = 0;
int redButtonState = 0;

const bool PRECISE =
    false; // Change to true if you need to define exact light positions
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
  u8g2.begin();

  u8g2.setFont(u8g2_font_logisoso32_tf); // set the target font to calculate the pixel width
  width = u8g2.getUTF8Width(text);    // calculate the pixel width of the text

  u8g2.setFontMode(0);    // enable transparent mode, which is faster

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

void display() {
    u8g2_uint_t x;

  u8g2.firstPage();
  do {

    // draw the scrolling text at current offset
    x = offset;
    u8g2.setFont(u8g2_font_logisoso32_tf);   // set the target font
    do {                // repeated drawing of the scrolling text...
      u8g2.drawUTF8(x, 32, text);     // draw the scolling text
      x += width;           // add the pixel width of the scrolling text
    } while ( x < u8g2.getDisplayWidth() );   // draw again until the complete display is filled

    u8g2.setFont(u8g2_font_logisoso32_tf);   // draw the current pixel width
    u8g2.setCursor(0, 64);
    u8g2.print(width);          // this value must be lesser than 128 unless U8G2_16BIT is set

  } while ( u8g2.nextPage() );

  offset -= 1;            // scroll by one pixel
  if ( (u8g2_uint_t)offset < (u8g2_uint_t) - width )
    offset = 0;             // start over again
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
  wait(200);
  strip.setPixelColor(pixelToLight, colors[pixelToLight]);
  strip.show();
  wait(1050);
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
