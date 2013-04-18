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
	pinMode(Global::brakeSensorPin1, INPUT);
	pinMode(Global::brakeSensorPin2, INPUT);
	pinMode(Global::amperageSensorPin, INPUT);
	pinMode(Global::throttleOutPin, OUTPUT);
	pinMode (Global::digiPotSelectPin, OUTPUT);
}

void BikeManager::digitalPotWrite(int value){
	  //take the SS pin low to select the chip:
	  digitalWrite(Global::digiPotSelectPin, LOW);
	  //send in the address and value via SPI:
	  SPI.transfer(0);
	  SPI.transfer(value);
	  // take the SS pin high to de-select the chip:
	  digitalWrite(Global::digiPotSelectPin, HIGH);
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
	if (Global::millisecRunning >= throttleLastProcessed + 20) {
		unsigned int throttleValueDesired = SignalProcessor::throttleSignal / 4;
		if (throttleValueDesired > 240) {
			throttleValueDesired = 240;
		}

		if (SignalProcessor::brakePulled /*|| ! SignalProcessor::isPedaling */) {
			throttleValueDesired = 0;
			throttleValueActual = 0;
		}
		// smoothen adjustment of throttle voltage
		throttleValueActual = increaseThrottleValue(throttleValueActual, 60, throttleValueDesired, 1);
//		Serial.print("adjust throttle:");
//		Serial.print(", desired=");
//		Serial.print(throttleValueDesired);
//		Serial.print(", actual=");
//		Serial.print(throttleValueActual);
//		Serial.println("");
//		analogWrite(Global::throttleOutPin, throttleValueActual);

		int tval = (int) throttleValueActual / 2;
//		Serial.print("tval=");
//		Serial.println(tval);

		  digitalPotWrite(tval);



		throttleLastProcessed = Global::millisecRunning;
	}
}

