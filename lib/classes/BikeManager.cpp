/*
 * BikeManager.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: stefan
 */

#include "BikeManager.h"
#include "../../main.h"

BikeManager::BikeManager() {
	// TODO Auto-generated constructor stub

}

BikeManager::~BikeManager() {
	// TODO Auto-generated destructor stub
}

unsigned long BikeManager::throttleLastProcessing = 0;
unsigned int BikeManager::currentThrottleValue = 0;
unsigned int BikeManager::targetThrottleValue = 0;

unsigned int BikeManager::throttleOutSignal() {
	unsigned int outSignal = SignalProcessor::throttleSignal;
	return outSignal;
}

void BikeManager::initPins() {
	pinMode(config::pasSensorPin, INPUT);
	digitalWrite(config::pasSensorPin, HIGH);
	pinMode(config::wheelSensorPin, INPUT);
	digitalWrite(config::wheelSensorPin, HIGH);
	pinMode(config::throttleSensorPin, INPUT);
	pinMode(config::brakeSensorPin, INPUT);
	pinMode(config::currentSensorPin, INPUT);
	pinMode(config::throttleOutPin, OUTPUT);
}

void BikeManager::adjustThrottle(int inValue) {
	if (Global::timeRunning >= throttleLastProcessing + 250) {
		targetThrottleValue = inValue / 4;
		if (SignalProcessor::brakePulled || ! SignalProcessor::isPedaling) {
			targetThrottleValue = 0;
			currentThrottleValue = 0;
		}
		// smoothen increasing of throttle voltage
		if (currentThrottleValue < targetThrottleValue) {
			if (currentThrottleValue < 100) {
				currentThrottleValue += 50;
			} else if (currentThrottleValue < 110) {
				currentThrottleValue += 3;
			} else if (currentThrottleValue < 120) {
				currentThrottleValue += 1;
			}
			else {
				currentThrottleValue ++;
			}
		}
		if (currentThrottleValue > targetThrottleValue) {
			currentThrottleValue = targetThrottleValue;
		}

//		currentThrottleValue = currentThrottleValue + (512 / (currentThrottleValue + 20));
//		if (currentThrottleValue > targetThrottleValue) {
//			currentThrottleValue = targetThrottleValue;
//		}

//		currentThrottleValue= 100;
		Serial.print("adjust throttle:");
		Serial.print("target value=");
		Serial.print(targetThrottleValue);
		Serial.print(", currentValue=");
		Serial.print(currentThrottleValue);
		Serial.println("");
		analogWrite(config::throttleOutPin, currentThrottleValue);
		throttleLastProcessing = Global::timeRunning;
	}
}

