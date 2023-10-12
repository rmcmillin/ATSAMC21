#include "sam.h"
#include "wdr.h"


void wdr_init(){
	//OSCULP32K).internal
	WDT->CONFIG.reg = WDT_CONFIG_PER_CYC16384;
	WDT->CTRLA.bit.ENABLE = 1;
}

void wdr_petDog(){
	if (!WDT->SYNCBUSY.bit.CLEAR){
		WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
	}
	else{
		//wait for previous clear command to sync
	}
}