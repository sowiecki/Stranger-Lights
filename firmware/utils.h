void checkButtonStates() {
  if (digitalRead(BUTTON_PIN_ONE) == LOW) {
    mode = DISPLAY_MESSAGE_MODE;
  } else if (digitalRead(BUTTON_PIN_TWO) == LOW) {
    mode = RAINBOW_MODE;
  } else if (digitalRead(BUTTON_PIN_THREE) == LOW) {
    mode = SPOOKY_MODE;
  } else if (digitalRead(BUTTON_PIN_FOUR) == LOW) {
    mode = UNIBEAM_MODE;
  }
}

void lazyDelay() {
  // Lazy way of checking button states while keeping paused for 1050ms
  delay(300);
  checkButtonStates();
  delay(250);
  checkButtonStates();
  delay(250);
  checkButtonStates();
  delay(250);
}
