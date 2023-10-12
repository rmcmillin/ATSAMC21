#include "sam.h"
#include "supc.h"

void supc_initVref(internalVref_t referenceSelect){
	//GCLK->PCHCTRL[SUPC_].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
		
	MCLK->APBAMASK.reg |= MCLK_APBAMASK_SUPC;
	
	switch (referenceSelect){
		case(VREF_1024):{
			SUPC->VREF.reg = SUPC_VREF_SEL_1V024 | SUPC_VREF_ONDEMAND;
			break;
			}
		case(VREF_2048):{
			SUPC->VREF.reg = SUPC_VREF_SEL_2V048 | SUPC_VREF_ONDEMAND;
			break;
			}
		case(VREF_4096):{
			SUPC->VREF.reg = SUPC_VREF_SEL_4V096 | SUPC_VREF_ONDEMAND;
			break;
			}
	}
}