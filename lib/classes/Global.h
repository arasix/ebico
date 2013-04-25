/*
 * Global.h
 *
 */

#include "Arduino.h"

#ifndef GLOBAL_H_
#define GLOBAL_H_

class Global {
public:
	static const boolean DEBUG = false;
	unsigned static long millisecRunning;
	unsigned static long microsecRunning;
	static const uint8_t pasSensorPin = 2;
	static const uint8_t wheelSensorPin = 3;
	static const uint8_t brakeSensorPin1 = 4;
	static const uint8_t brakeSensorPin2 = 5;
	static const uint8_t throttleSensorPin = A0;
	static const uint8_t amperageSensorPin = A1;
	static const uint8_t pasNrMagnets = 12;
	static const uint8_t digiPotSelectPin = 10;
};

#endif /* GLOBAL_H_ */
