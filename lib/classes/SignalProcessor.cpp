/*
 * SignalProcessor.cpp
 *
 */

#include "SignalProcessor.h"
#include "config.h"

SignalProcessor::SignalProcessor() {
}

unsigned long SignalProcessor::pasTime = 0;
unsigned long SignalProcessor::pasLastTime = 0;
unsigned int SignalProcessor::pasTimeOn = 0;
unsigned int SignalProcessor::pasTimeOff = 0;
unsigned int SignalProcessor::pasSignalCount = 0;
byte pasRPM = 0;

void SignalProcessor::startCollect() {
	Serial.println("call: SignalProcessor::collect()");
	pinMode(config::pasSensorPin, INPUT);
	digitalWrite(config::pasSensorPin, HIGH);
	pinMode(config::wheelSensorPin, INPUT);
	digitalWrite(config::wheelSensorPin, HIGH);
	pinMode(config::throttleSensorPin, INPUT);
	pinMode(config::brakeSensorPin, INPUT);
	attachInterrupt(0, SignalProcessor::collectPasSignals, CHANGE);
	attachInterrupt(1, SignalProcessor::collectWheelSignals, RISING);
}

void SignalProcessor::stopCollect() {
	detachInterrupt(0);
	detachInterrupt(1);
}

void SignalProcessor::collectWheelSignals() {
	Serial.print("wheel signal received=");
	Serial.println(digitalRead(config::wheelSensorPin));
}

void SignalProcessor::collectPasSignals() {
	Serial.print("pas signal received=");
	Serial.println(digitalRead(config::pasSensorPin));
	SignalProcessor::pasTime = millis();
	if (digitalRead(config::pasSensorPin) == 1) {
		SignalProcessor::pasTimeOn += SignalProcessor::pasTime - SignalProcessor::pasLastTime;
		SignalProcessor::pasSignalCount++;
	} else {
		SignalProcessor::pasTimeOff += SignalProcessor::pasTime - SignalProcessor::pasLastTime;
	}
	SignalProcessor::pasLastTime = SignalProcessor::pasTime;

}

SignalProcessor::~SignalProcessor() {
	stopCollect();
}
