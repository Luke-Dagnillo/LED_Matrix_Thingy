#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>


// Definitions
#define LED_PIN 5     // GPIO Pin where LED strip is connected
#define BUTTON_PIN 7	//GPIO Pin where button is connected
#define NUM_LEDS 100
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


void setup() {
	// put your setup code here, to run once:
	delay(2000); // Wait 2 seconds for serial monitor stabilization
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	strip.begin();
	strip.show();
	Serial.begin(9600);
	Serial.println("Starting...");
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
}


// put function definitions here:

void displayDefaultPattern() { 
	// Rainbow Pattern
	for (int i = 0; i < strip.numPixels(); i++) {
		int pixelHue = (i * 256 / strip.numPixels()) + (millis() / 10);
		strip.setPixelColor(i, strip.ColorHSV(pixelHue));
	}

	strip.show();
}

void displayMicPattern(){
	int micValue = analogRead(MIC_PIN);
	int brightness = map(micValue, 0, 1023, 0, 255);

	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, strip.Color(brightness, 0, 255 - brightness));
	}

	strip.show();
}