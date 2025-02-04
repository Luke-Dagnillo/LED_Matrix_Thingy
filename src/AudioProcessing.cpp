#include "AudioProcessing.h"

// Constructor to set the microphone pin
AudioProcessing::AudioProcessing(uint8_t micPin) {
    _micPin = micPin;
}

// Initialize microphone pin
void AudioProcessing::begin() {
    pinMode(_micPin, INPUT);
}

// Read the raw signal from the microphone
int AudioProcessing::readMicrophone() {
    return analogRead(_micPin); // Perform an ADC read from the microphone pin
}

// Calculate and return normalized amplitude
int AudioProcessing::getAmplitude() {
    const int numSamples = 50;   // Number of samples to average
    const int centerValue = 1500; // Adjust center value based on observed data
    int total = 0;

    for (int i = 0; i < numSamples; i++) {
        int sample = readMicrophone();
        total += abs(sample - centerValue); // Use the new center value
        delayMicroseconds(50);             // Small delay for sampling
    }

    int average = total / numSamples;

    // Adjust based on observed microphone output range
    const int noiseThreshold = 7;   // Ignore low-level noise
    if (average < noiseThreshold) {
        return 0;  // Return 0 if below noise threshold
    }

    // Map amplitude to 0-9 range for matrix
    int normalizedAmplitude = map(average, 0, 300, 0, 9);  // Adjust range based on observed variation
    return constrain(normalizedAmplitude, 0, 9);
}