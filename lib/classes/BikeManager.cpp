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

float  BikeManager::increaseThrottleValue(float actualValue, unsigned int min = 80, unsigned int max = 120, float slope = 1) {
	if (actualValue < min) {
		actualValue = min;
	}
	//float addValue = (max - actualValue) * (max * 0.0005 * (1/slope));
	float addValue =  0.2;
	actualValue += addValue;
	if (actualValue > max) {
		actualValue = max;
	}
	return actualValue;
}

void BikeManager::adjustThrottle() {
	if (Global::millisecRunning >= throttleLastProcessed + 50) {
		unsigned int throttleValueDesired = SignalProcessor::throttleSignal / 2;
		if (SignalProcessor::brakePulled  || ! SignalProcessor::isPedaling ) {
			throttleValueDesired = 0;
			throttleValueActual = 0;
		}
		// smoothen adjustment of throttle voltage
		int tresholdVal =  ((int) SignalProcessor::wheelRPM / 10) + 42;
		throttleValueActual = increaseThrottleValue(throttleValueActual, tresholdVal, throttleValueDesired, 1);
		if (Global::DEBUG) {
//			Serial.print("adjust throttle:");
//			Serial.print(", desired=");
//			Serial.print(throttleValueDesired);
//			Serial.print(", actual=");
//			Serial.print(throttleValueActual);
//			Serial.println("");
		}
//		analogWrite(Global::throttleOutPin, throttleValueActual)
		int tval = (int) throttleValueActual;
		digitalPotWrite(tval);
		throttleLastProcessed = Global::millisecRunning;
	}
}

