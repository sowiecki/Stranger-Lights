// Change each letter to the actual index of that letter's light position
void lightLetter(int pixel) {
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
