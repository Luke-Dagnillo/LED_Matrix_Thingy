#ifndef ADUIO_PROCESSING_H
#define ADUIO_PROCESSING_H

#include <Arduino.h>


class AudioProcessing {
public:
    AudioProcessing(uint8_t micPin); // Constructor to set up the microphone pin
    void begin();                       // Initialize the ADC
    int getAmplitude();                 // return normalized amplitude

private:
    uint8_t _micPin;          // Pin connected to micorphone
    int readMicrophone();     // raw ADC reading from mic
};



#endif