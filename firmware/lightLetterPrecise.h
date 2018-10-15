// Change each letter to the actual index of that letter's light position
void lightLetter(int pixel) {
  Serial.println("Letter: " + String(pixel));

  if (pixel == 0) {
    flashPixel(60); // a
  } else if (pixel == 1) {
    flashPixel(58); // b
  } else if (pixel == 2) {
    flashPixel(57); // c
  } else if (pixel == 3) {
    flashPixel(54); // d
  } else if (pixel == 4) {
    flashPixel(53); // e
  } else if (pixel == 5) {
    flashPixel(51); // f
  } else if (pixel == 6) {
    flashPixel(50); // g
  } else if (pixel == 7) {
    flashPixel(49); // h
  } else if (pixel == 8) {
    flashPixel(26); // i
  } else if (pixel == 9) {
    flashPixel(29); // j
  } else if (pixel == 10) {
    flashPixel(30); // k
  } else if (pixel == 11) {
    flashPixel(32); // l
  } else if (pixel == 12) {
    flashPixel(34); // m
  } else if (pixel == 13) {
    flashPixel(36); // n
  } else if (pixel == 14) {
    flashPixel(38); // o
  } else if (pixel == 15) {
    flashPixel(39); // p
  } else if (pixel == 16) {
    flashPixel(41); // q
  } else if (pixel == 17) {
    flashPixel(19); // r
  } else if (pixel == 18) {
    flashPixel(17); // s
  } else if (pixel == 19) {
    flashPixel(15); // t
  } else if (pixel == 20) {
    flashPixel(14); // u
  } else if (pixel == 21) {
    flashPixel(12); // v
  } else if (pixel == 22) {
    flashPixel(11); // w
  } else if (pixel == 23) {
    flashPixel(9); // x
  } else if (pixel == 24) {
    flashPixel(7); // y
  } else if (pixel == 25) {
    flashPixel(5); // z
  }
}
