/*
 * ParameterProcessor.cpp
 *
 */

#include "ParameterProcessor.h"
#include "config.h"

ParameterProcessor::ParameterProcessor() {
}

void ParameterProcessor::startCollect() {
	pinMode(config::pasSensorPin, INPUT);
	digitalWrite(config::pasSensorPin,HIGH);
	pinMode(config::wheelSensorPin, INPUT);
	digitalWrite(config::wheelSensorPin,HIGH);
	pinMode(config::throttleSensorPin, INPUT);
	pinMode(config::brakeSensorPin, INPUT);
	Serial.println("call: ParameterProcessor::collect()");
	attachInterrupt(0, collectPasSignals, CHANGE);
	attachInterrupt(1, collectWheelSignals, RISING);
}

void ParameterProcessor::stopCollect() {
	detachInterrupt(0);
	detachInterrupt(1);
}

void ParameterProcessor::collectWheelSignals() {
	Serial.print("wheel signal received=");
	Serial.println(digitalRead(config::wheelSensorPin));
}

void ParameterProcessor::collectPasSignals() {
	Serial.print("pas signal received=");
	Serial.println(digitalRead(config::pasSensorPin));

}

ParameterProcessor::~ParameterProcessor() {
	stopCollect();
}
