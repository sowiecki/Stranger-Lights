#undef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#undef min
inline int min(int a, int b) { return ((a) < (b) ? (a) : (b)); }
#define UP "UP"
#define DOWN "DOWN"

int red = 100;
int blue = 0;
String blueDirection = UP;
String redDirection = DOWN;
String direction = UP;
int valueToLight = 0;

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

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

uint32_t setColorIntensity(uint8_t g, uint8_t r, uint8_t b, uint8_t i) {
  uint8_t GREEN = g - 1 >= 0 && g - i <= 255 ? g - i : 0;
  uint8_t RED = r - 1 >= 0 && r - i <= 255 ? r - i : 0;
  uint8_t BLUE = b - 1 >= 0 && b - i <= 255 ? b - i : 0;

  return strip.Color(RED, GREEN, BLUE);
}

void setStripColor(uint32_t color, uint32_t altColor = strip.Color(255, 255, 255), int8_t set = 1) {
  bool invert = set < 0;

  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    if (i % set == 0) {
      strip.setPixelColor(i, invert ? altColor : color);
    } else {
      strip.setPixelColor(i, invert ? color : altColor);
    }
  }

  strip.show();
}

void pulseColor(
    uint8_t g,
    uint8_t r,
    uint8_t b,
    uint8_t rate,
    uint32_t altColor = strip.Color(255, 255, 255),
    int8_t set = 1) {
  int8_t PULSE_MAX = max(max(r, g), b);
  int8_t minRGB = min(min(r, g), b);
  int8_t PULSE_MIN = minRGB == PULSE_MAX ? 0 : minRGB;
  uint32_t color;

  for (uint8_t i = PULSE_MIN; i <= PULSE_MAX; i += rate) {
    color = setColorIntensity(g, r, b, PULSE_MAX - i);
    checkButtonStates();

    setStripColor(color, altColor, set);
    delay(10);
  }

  for (uint8_t i = PULSE_MIN; i <= PULSE_MAX; i += rate) {
    color = setColorIntensity(g, r, b, i);
    checkButtonStates();

    setStripColor(color, altColor, set);
    delay(10);
  }
}

// 0,128,128 Purple (GRB)
// 140,255,128 Orange (GRB)
void spooky() {
#if (OLED_ENABLE)
  resetOLED();
  display.clearDisplay();
  setText("Mode: " + String(mode), 0, 8);
#endif

  strip.setBrightness(200);
  pulseColor(0, 100, 100, 1, strip.Color(255, 20, 60), -4);
}

void unibeam() {
#if (OLED_ENABLE)
  resetOLED();
  display.clearDisplay();
  setText("Mode: " + String(mode), 0, 8);
#endif

  strip.setBrightness(255);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    checkButtonStates();
    strip.setPixelColor(i, 255, 255, 255, 255);
  }
  strip.show();

  for (uint16_t i = 0; i < 100; i++) {
    checkButtonStates();
    delay(50);
  }
}

// From Adafruit NeoPixel example
void rainbow(uint8_t wait) {
#if (OLED_ENABLE)
  resetOLED();
  display.clearDisplay();
  setText("Mode: " + String(mode), 0, 8);
#endif

  uint16_t i, j;
  strip.setBrightness(255);

  for (j = 0; j <= 255; j++) {
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

void flashPixel(int8_t pixel, int8_t wait = 600) {
  strip.setPixelColor(pixel, colors[pixel]);

  for (int i = 25; i < MAX_BRIGHTNESS; i += 6) {
    checkButtonStates();
    strip.setBrightness(i);
    strip.show();

    delay(1);
  }

  delay(700);

  for (int i = MAX_BRIGHTNESS; i > 0; i -= 6) {
    checkButtonStates();
    strip.setBrightness(i);
    strip.show();

    delay(1);
  }

  strip.setPixelColor(pixel, 0);
  strip.show();

  delay(wait);
}
