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
    strip.show();
  }
}

uint32_t setColorIntensity(uint8_t r, uint8_t g, uint8_t b, uint8_t i) {
  uint8_t RED = r - 1 >= 0 ? r - i : 0;
  uint8_t GREEN = g - 1 >= 0 ? g - i : 0;
  uint8_t BLUE = b - 1 >= 0 ? b - i : 0;

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

    strip.show();
  }
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
    color = setColorIntensity(r, g, b, PULSE_MAX - i);
    checkButtonStates();

    setStripColor(color, altColor, set);
    delay(40);
  }

  for (uint8_t i = PULSE_MIN; i <= PULSE_MAX; i += rate) {
    color = setColorIntensity(r, g, b, i);
    checkButtonStates();

    setStripColor(color, altColor, set);
    delay(40);
  }
}

void spooky() {
  pulseColor(50, 0, 0, 30, strip.Color(50, 90, 0), -4);
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
