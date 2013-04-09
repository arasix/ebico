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
};

#endif /* BIKEMANAGER_H_ */
