#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Definitions
#define LED_PIN 5 
#define NUM_LEDS 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// put function declarations here:


void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show(); // Initialize all LEDs to off
}

void loop() {
  // put your main code here, to run repeatedly:
  // Color wipe effect
  for (int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // red color
    strip.show();
    delay(50);
  }

}

// put function definitions here:
