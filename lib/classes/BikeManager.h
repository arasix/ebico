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
	static uint8_t throttleValueActual;
	unsigned static long throttleLastProcessed;
};

#endif /* BIKEMANAGER_H_ */
