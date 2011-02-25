#ifndef I2CBUS_H
#define I2CBUS_H

#include "Singleton.h"

class I2cBus : public Singleton<I2cBus>{
	public:
		void onOpen(char* device);
		void onClose(int descripteurDevice);
		
	private:
		
};


#endif
