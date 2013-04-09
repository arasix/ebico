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
uint8_t BikeManager::throttleValueActual = 0;

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
	if (Global::millisecRunning >= throttleLastProcessed + 250) {
		uint8_t throttleValueDesired = SignalProcessor::throttleSignal / 4;
		uint8_t throttleValueNew = 0;
//		if (SignalProcessor::brakePulled || ! SignalProcessor::isPedaling) {
//			throttleValueDesired = 0;
//			throttleValueActual = 0;
//		}
		// smoothen adjustment of throttle voltage
		if (throttleValueActual < throttleValueDesired) {
			if (throttleValueActual < 90) {
				throttleValueNew = throttleValueActual + 45;
				if (throttleValueNew > 100) {
					throttleValueNew = 100;
				}
			} else if (throttleValueActual < 110) {
				throttleValueNew = throttleValueActual + 3;
			} else if (throttleValueActual < 110) {
				throttleValueNew = throttleValueActual + 3;
			}
			else {
				throttleValueNew = throttleValueActual + 1;
			}
		}
		if (throttleValueNew > throttleValueDesired) {
			throttleValueNew = throttleValueDesired;
		}
//		throttleValueActual = throttleValueActual + (512 / (throttleValueActual + 20));
//		if (throttleValueActual > throttleValueDesired) {
//			throttleValueActual = throttleValueDesired;
//		}
//		throttleValueActual= 100;
//		Serial.print("adjust throttle:");
//		Serial.print("target value=");
//		Serial.print(throttleValueDesired);
//		Serial.print(", amperageValue=");
//		Serial.print(throttleValueActual);
//		Serial.println("");
		analogWrite(Global::throttleOutPin, throttleValueNew);
		throttleValueActual = throttleValueNew;
		throttleLastProcessed = Global::millisecRunning;
	}
}

