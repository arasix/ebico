/*
 * config.h
 *
 */

#ifndef CONFIG_H_
#define CONFIG_H_

namespace config {
	static const uint8_t pasSensorPin = 2;
	static const uint8_t wheelSensorPin = 3;
	static const uint8_t brakeSensorPin = 4;
	static const uint8_t throttleSensorPin = A0;
	static const uint8_t currentSensorPin = A1;
	static const uint8_t pasNrMagnets = 12;
	static const uint8_t throttleOutPin = 5;
};

#endif /* CONFIG_H_ */
