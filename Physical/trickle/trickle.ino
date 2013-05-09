/*
 * An Arduino controller for LED strings based on the SM16716 controller.
 *
 * By Mike Tsao (github.com/sowbug). Copyright 2012, all rights reserved.
 *
 * You might see these being sold as WS2801 strands, and you're wondering why
 * your off-the-shelf WS2801 code isn't working. This sketch will restore
 * your confidence that your lights aren't broken.
 *
 * This code isn't the fastest, but it is just about the simplest you'll
 * find. For performance, investigate code that uses hardware SPI.
 *
 * Beware that everyone seems to use different wiring color schemes. I
 * found the order of the wires is more reliable. Looking at the IC side
 * of the board, you'll find from left to right: V+, Data, Clock, Ground.
 */

#define DATA_PIN (11)
#define CLOCK_PIN (13)

#define LIGHT_COUNT (50)

uint32_t pixels[LIGHT_COUNT];

#define pinModeFast(x, y) pinMode(x, y)
#define digitalWriteFast(x, y) digitalWrite(x, y)

void setup() {
  Serial.begin(9600);
  Serial.write("Set up!!!");
  pinModeFast(DATA_PIN, OUTPUT);
  pinModeFast(CLOCK_PIN, OUTPUT);
  digitalWriteFast(DATA_PIN, LOW);
  digitalWriteFast(CLOCK_PIN, LOW);
}

static void set_pixel(uint8_t index, uint32_t color) {
  pixels[index] = color & 0x00FFFFFF;
}

static void set_pixel_rgb(uint8_t index, uint32_t r, uint32_t g, uint32_t b) {
  set_pixel(index, r | (g << 8) | (b << 16));
}

static void toggle_clock() {
  digitalWriteFast(CLOCK_PIN, HIGH);
  digitalWriteFast(CLOCK_PIN, LOW);
}

static void write_pixel(uint8_t i) {
  const uint32_t MASK = ((uint32_t)(1) << 24);
  uint32_t p = pixels[i] | MASK;
  int j = 25;
  while (j--) {
    digitalWriteFast(DATA_PIN, (p & MASK) ? HIGH : LOW);
    toggle_clock();
    p <<= 1;
  }
}

static void write_blank_pixel() {
  int j = 25;
  while (j--) {
    digitalWriteFast(DATA_PIN, 0);
    toggle_clock();
  }
}

static void show() {
  digitalWriteFast(DATA_PIN, LOW);
  for (int i = 0; i < 50; i++) {
    toggle_clock();
  }
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    write_pixel(i);
  }
  write_blank_pixel();
  delay(1);
}

static void solid_color(uint32_t color) {
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    set_pixel(i, color);
  }
}

static uint32_t simple_color(uint8_t color) {
  switch (color) {
  case 0:
    return 0x00ff0000;
  case 1:
    return 0x0000ff00;
  case 2:
    return 0x000000ff;
  case 3:
    return 0x00ffff00;
  case 4:
    return 0x00ff00ff;
  case 5:
    return 0x0000ffff;
  case 6:
    return 0x00ffffff;
  default:
    return 0x00330000;
  }
}

static uint32_t blue_color(uint8_t color) {
  switch (color) {
  case 0:
    return 0x004682B4;
  case 1:
    return 0x004682B4;
  case 2:
    return 0x00B0C4DE;
  case 3:
    return 0x00B0E0E6;
  case 4:
    return 0x00ff00ff;
  case 5:
    return 0x0087CEEB;
  case 6:
    return 0x0087CEFA;
  default:
    return 0x0000BFFF;
  }
}

static uint32_t rand_color() {
  return simple_color(rand() % 7);
}

static uint32_t rand_color_except(uint32_t color) {
  uint32_t result_color;
  do {
    result_color = simple_color(rand() % 7);
  } while (result_color == color);
  return result_color;
}

static uint32_t get_dimmer_mask(uint32_t value) {
  return value | (value << 8) | (value << 16);
}

static void scroll(int8_t delta) {
  size_t bytes_to_move = (LIGHT_COUNT - delta) * sizeof(uint32_t);
  if (delta > 0) {
    memmove(&pixels[delta], &pixels[0], bytes_to_move);
  } else {
    memmove(&pixels[0], &pixels[-delta], bytes_to_move);
  }
}

static void wipe(uint8_t delay_msec) {
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    scroll(1);
    set_pixel(0, 0);
    show();
    delay(delay_msec);
  }
}

String getHex(int num) {
      char tmp[16];
      char format[128];

//      sprintf(format, "0x%%.%dX", precision);
      sprintf(format, "%%.%dX", 2);

      sprintf(tmp, format, num);
      Serial.println(tmp);
      return String(tmp);
}

void loop() {
  uint32_t color = 0;
    int val1 = analogRead(2) / 4;
//    solid_color(val1);
//    show();
    for (uint8_t jj= 0; jj <50; ++jj){
      set_pixel_rgb(jj, val1,255-val1,val1);
      show();
    }

}
