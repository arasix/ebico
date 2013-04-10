/*
 * BikeManager.h
 *
 */
#include "Arduino.h"

#ifndef BIKEMANAGER_H_
#define BIKEMANAGER_H_

class BikeManager {
public:
	BikeManager();
	virtual ~BikeManager();
	void initPins();
	void adjustThrottle();
private:
	static float throttleValueActual;
	unsigned static long throttleLastProcessed;
	float increaseThrottleValue(float actualValue, unsigned int min, unsigned int max, float slope);
};

#endif /* BIKEMANAGER_H_ */
