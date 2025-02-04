#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "AudioProcessing.h"


// Definitions
#define LED_PIN 5     // GPIO Pin where LED strip is connected  (D2)
#define BUTTON_PIN 7	// D7 Pin where button is connected
#define NUM_LEDS 90
#define MIC_PIN A0    // Analog pin connected to microphone

// Function Declarations
void displayDefaultPattern();
void displayMicPattern();

//LED Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// States
enum Mode {DEFAULT_MODE, MIC_MODE};
Mode currentMode = DEFAULT_MODE;

// Debounce vairables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50ms debounce delay
bool buttonState = HIGH;
bool lastButtonState = HIGH;

// Audio Processing setup
AudioProcessing audio(MIC_PIN);



void setup() {
	// // put your setup code here, to run once:
	delay(2000); // Wait 2 seconds for serial monitor stabilization
	// pinMode(BUTTON_PIN, INPUT_PULLUP);
	// strip.begin();
	// strip.show();
	// Serial.begin(9600);
	// Serial.println("Starting...");

	audio.begin();
    Serial.begin(9600); 
}

void loop() {
	// put your main code here, to run repeatedly:
	
	// button debounce logic
	bool reading = digitalRead(BUTTON_PIN);
	if (reading != lastButtonState) {
		lastDebounceTime = millis();
	} 
	if ((millis() - lastDebounceTime) > debounceDelay) {
		if (reading == LOW && buttonState == HIGH) { // Button is pressed
			currentMode = (currentMode == DEFAULT_MODE) ? MIC_MODE : DEFAULT_MODE;
			Serial.println(currentMode == DEFAULT_MODE ? "Default Mode" : "Mic Mode");
		}
		buttonState = reading;
	}
	lastButtonState = reading;

	// Display mode specific patterns
	if (currentMode == DEFAULT_MODE) {
		displayDefaultPattern();
	}
	else if (currentMode == MIC_MODE) {
		displayMicPattern();
	}



	// int amplitude = audio.getAmplitude();
    // Serial.println(amplitude);


	delay(50);
}


// put function definitions here:

void displayDefaultPattern() { 
	// Rainbow Pattern
	int rainbowSpeed = 1; // Lower values = faster, higher values = slower

	for (int i = 0; i < strip.numPixels(); i++) {
		int pixelHue = (i * 256 / strip.numPixels()) + (millis() / rainbowSpeed);
		strip.setPixelColor(i, strip.ColorHSV(pixelHue));
	}

	strip.show();
}

void displayMicPattern(){
	int amplitude = audio.getAmplitude();  // get sound amplitude

	// clear the matrix

	// define the color gradient based on intensity
	uint32_t colors[] = {
		strip.Color(0, 0, 255),   // Blue (low)
        strip.Color(0, 255, 255), // Cyan
        strip.Color(0, 255, 0),   // Green
        strip.Color(255, 255, 0), // Yellow
        strip.Color(255, 0, 0)    // Red (high)
	};

	// map amplitude to 9 rows of the 9x10 matrix
	for (int col = 0; col < 10; col++) {
		for (int row = 0; row < amplitude; row++) {  // light up to "amplitude height"
			int ledIndex = (row * 10) + col; // convert (row, col) to LED index
			uint32_t color = colors[min(row / 2, 4)];  // choose color gradient
			strip.setPixelColor(ledIndex, color);
		}
	}

	strip.show();  // Update LED strip
}
