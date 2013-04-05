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
byte SignalProcessor::pasRPM = 0;
unsigned long SignalProcessor::pasLastProcessing = 0;
bool SignalProcessor::pasDirection;
bool SignalProcessor::isPedaling = false;

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

void SignalProcessor::processSignals() {
	if (global::millisRunning >= pasLastProcessing + 1000) {
		isPedaling = pasLastTime > global::millisRunning;
		pasDirection = pasTimeOn < pasTimeOff;
		Serial.print("process pas: ");
		  Serial.print("signal  on=");
		  Serial.print(pasTimeOn);
		  Serial.print(", off=");
		  Serial.print(pasTimeOff);
		  Serial.print(", direction=");
		  if (pasTimeOn < pasTimeOff) {
		    Serial.print("forward");
		  } else {
		    Serial.print("backward");
		  }
		  Serial.println("");
		pasLastProcessing = global::millisRunning;
	}
}

SignalProcessor::~SignalProcessor() {
	stopCollect();
}
