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
