void checkButtonStates() {
  // if (digitalRead(BUTTON_PIN_ONE) == LOW) {
  //   mode = RAINBOW_MODE;
  // } else if (digitalRead(BUTTON_PIN_TWO) == LOW) {
  //   mode = SPOOKY_MODE;
  // } else if (digitalRead(BUTTON_PIN_THREE) == LOW) {
  //   mode = DISPLAY_MESSAGE_MODE;
  // } else if (digitalRead(BUTTON_PIN_FOUR) == LOW) {
  //   mode = UNIBEAM_MODE;
  // }
}

void initColors() {
  for (int i = 0; i < strip.numPixels(); i++) {
    int rgb = i % 5;
    switch (rgb) {
    case 0:
      colors[i] = strip.Color(MAX_BRIGHTNESS, 0, 0); // green
      break;
    case 1:
      colors[i] = strip.Color(0, MAX_BRIGHTNESS, 0); // red
      break;
    case 2:
      colors[i] = strip.Color(0, 0, MAX_BRIGHTNESS); // blue
      break;
    case 3:
      colors[i] = strip.Color(MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS); // magenta
      break;
    case 4:
      colors[i] = strip.Color(MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0); // yellow
      break;
    }
  }
}
