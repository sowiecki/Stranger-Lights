void checkButtonStates() {
  if (digitalRead(BUTTON_PIN_GREEN) == LOW) {
    mode = DISPLAY_MESSAGE_MODE;
  } else if (digitalRead(BUTTON_PIN_RED) == LOW) {
    mode = RAINBOW_MODE;
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
