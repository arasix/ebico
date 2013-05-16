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


float BikeManager::limitThrottleVal(float rpm, int offset) {
	int max = 102;
	int releaseFromRPM = 350;
	if (rpm > releaseFromRPM) {
		return max;
	}
	max = max - offset;
	float val = ((max - max * (rpm / releaseFromRPM)) - max ) *  -1 + offset;
//	Serial.print(", rpm=");
//	Serial.print(rpm);
//	Serial.print(", val=");
//	Serial.print(val);
//	Serial.println("");
	return  val;

}

void BikeManager::adjustThrottle() {
	if (Global::millisecRunning >= throttleLastProcessed + 50) {
		unsigned int throttleValueDesired = SignalProcessor::throttleSignal / 2.2;
		if (SignalProcessor::brakePulled  || ! SignalProcessor::isPedaling ) {
			throttleValueDesired = 0;
			throttleValueActual = 0;
		}
		// smoothen adjustment of throttle voltage
		int motorStartOffset = 45;
		int pullUpValue =  ((int) SignalProcessor::wheelRPM / 10);
		throttleValueActual = increaseThrottleValue(throttleValueActual, (motorStartOffset + pullUpValue) , throttleValueDesired, 1);
//		int topValue = limitThrottleVal(SignalProcessor::wheelRPM, motorStartOffset);
//		if (throttleValueActual > topValue) {
//			throttleValueActual = topValue;
//		}
//		if (Global::DEBUG) {
//			Serial.print("adjust throttle:");
//			Serial.print(", desired=");
//			Serial.print(throttleValueDesired);
//			Serial.print(", actual=");
//			Serial.print(throttleValueActual);
//			Serial.println("");
//		}
		int tval = (int) throttleValueActual;
		digitalPotWrite(tval);
		throttleLastProcessed = Global::millisecRunning;
	}
}

