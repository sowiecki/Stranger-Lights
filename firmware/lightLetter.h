void lightLetter(int8_t pixel) {
  if (pixel >= 8 && pixel <= 16) {
    flashPixel(pixel + 1);
  } else {
    flashPixel(25 - pixel);
  }
}
