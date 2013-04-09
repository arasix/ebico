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

void BikeManager::adjustThrottle() {
	if (Global::millisecRunning >= throttleLastProcessed + 100) {
		unsigned int throttleValueDesired = SignalProcessor::throttleSignal / 4;
//		if (SignalProcessor::brakePulled || ! SignalProcessor::isPedaling) {
//			throttleValueDesired = 0;
//			throttleValueActual = 0;
//		}
		// smoothen adjustment of throttle voltage
		if (throttleValueActual < throttleValueDesired) {
			if (throttleValueActual < 90) {
				throttleValueActual += 20;
			} else if (throttleValueActual < 110) {
				throttleValueActual += 1;
			} else if (throttleValueActual < 110) {
				throttleValueActual += .8;
			}
			else {
				throttleValueActual += .5;
			}
		}
		if (throttleValueActual > throttleValueDesired) {
			throttleValueActual = throttleValueDesired;
		}
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

