/*
 * BikeManager.h
 *
 *  Created on: Apr 5, 2013
 *      Author: stefan
 */

#ifndef BIKEMANAGER_H_
#define BIKEMANAGER_H_

class BikeManager {
public:
	BikeManager();
	virtual ~BikeManager();
	unsigned int throttleOutSignal();
	void initPins();
	void adjustThrottle(int value);
private:
	unsigned static int currentThrottleValue;
	unsigned static int targetThrottleValue;
	unsigned static long throttleLastProcessing;
};

#endif /* BIKEMANAGER_H_ */
