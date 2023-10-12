#include "sam.h"
#include "rstc.h"

resetSource_t rstc_getResetSource(){
	uint8_t source = RSTC->RCAUSE.reg;
	
	if (source & (1 << POWER_ON_RESET)){
		return POWER_ON_RESET;
	}
	else if (source & (1 << BODCORE)){
		return BODCORE;
	}
	else if (source & (1 << BODVDD)){
		return BODVDD;
	}
	else if (source & (1 << EXTERNAL_RESET)){
		return EXTERNAL_RESET;
	}
	else if (source & (1 << WATCHDOG)){
		return WATCHDOG;
	}
	else if (source & (1 << SOFTWARE_RESET)){
		return SOFTWARE_RESET;
	}	
	
	return -1; //error
}