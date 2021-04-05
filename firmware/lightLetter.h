void lightLetter(int8_t pixel, int8_t wait = 600) {
  if (pixel >= 8 && pixel <= 16) {
    flashPixel(pixel + 1, wait);
  } else {
    flashPixel(25 - pixel, wait);
  }
}
