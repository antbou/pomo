/// @file    Pomodoro.ino
/// @brief   Simple Pomodoro timer with WS2812 LED strip

#include <Arduino.h>
#include <FastLED.h>

// -------------------- Configuration --------------------
#define DATA_PIN 7
#define NUM_LEDS 25
#define SESSION_COUNT 4  // number of work+break cycles
#define WORK_MINUTES 25
#define BREAK_MINUTES 5
#define BRIGHTNESS 64  // 0..255
#define BTN_PIN 3

CRGB leds[NUM_LEDS];


void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);
  Serial.begin(9600);
}

void loop() {
  // exit early if button not pressed
  if (digitalRead(BTN_PIN) != LOW) {
    return;
  }

  delay(50); // simple debounce

  // exit if noise was detected (button released after debounce)
  if (digitalRead(BTN_PIN) != LOW) {
    return;
  }

  for (int i = 0; i < SESSION_COUNT; i++) {
    run_work_session(WORK_MINUTES);
    run_break_session(BREAK_MINUTES);
  }

  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(500);
  FastLED.clear(true);
}



long minutes_to_ms(int minutes) {
  return minutes * 60000;
}

void run_work_session(int minutes) {
  long step_delay = minutes_to_ms(minutes) / NUM_LEDS;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(step_delay);
  }
}

void run_break_session(int minutes) {
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(minutes_to_ms(minutes));
  FastLED.clear(true);
}
