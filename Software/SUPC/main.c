#define F_CPU 4000000UL

#include "sam.h"
#include "dac.h"
#include "supc.h"

int main(void)
{    
    SystemInit();	
	
	supc_initVref(VREF_4096);
	
	dac_init();
	dac_setMilliVoltage(2500);
	
    while (1) 
    {
		
    }
}
