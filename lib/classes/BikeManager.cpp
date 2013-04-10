/*
 * BikeManager.cpp
 *
 */

#include "BikeManager.h"
#include "../../main.h"

BikeManager::BikeManager() {
}

BikeManager::~BikeManager() {
}

unsigned long BikeManager::throttleLastProcessed = 0;
float BikeManager::throttleValueActual = 0;

void BikeManager::initPins() {
	pinMode(Global::pasSensorPin, INPUT);
	digitalWrite(Global::pasSensorPin, HIGH);
	pinMode(Global::wheelSensorPin, INPUT);
	digitalWrite(Global::wheelSensorPin, HIGH);
	pinMode(Global::throttleSensorPin, INPUT);
	pinMode(Global::brakeSensorPin, INPUT);
	pinMode(Global::amperageSensorPin, INPUT);
	pinMode(Global::throttleOutPin, OUTPUT);
}

float  BikeManager::increaseThrottleValue(float actualValue, unsigned int min = 60, unsigned int max = 255, float slope = 1) {
	if (actualValue < min) {
		actualValue = min;
	}
	float addValue = (max - actualValue) * (max * 0.0001 * (1/slope));
	actualValue += addValue;
	if (actualValue > max) {
		actualValue = max;
	}
	return actualValue;
}

void BikeManager::adjustThrottle() {
	if (Global::millisecRunning >= throttleLastProcessed + 10) {
		//unsigned int throttleValueDesired = SignalProcessor::throttleSignal / 4;
		unsigned int throttleValueDesired = 255;
//		if (SignalProcessor::brakePulled || ! SignalProcessor::isPedaling) {
//			throttleValueDesired = 0;
//			throttleValueActual = 0;
//		}
		// smoothen adjustment of throttle voltage
		throttleValueActual = increaseThrottleValue(throttleValueActual, 60, throttleValueDesired, 20);
		Serial.print("adjust throttle:");
		Serial.print(", desired=");
		Serial.print(throttleValueDesired);
		Serial.print(", actual=");
		Serial.print(throttleValueActual);
		Serial.println("");
		analogWrite(Global::throttleOutPin, (int) throttleValueActual);
		throttleLastProcessed = Global::millisecRunning;
	}
}

