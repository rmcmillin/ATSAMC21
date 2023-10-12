#define F_CPU 4000000UL

#include "sam.h"
#include "dac.h"
#include "uart.h"

int main(void)
{    
    SystemInit();
	uart_init(115200, F_CPU);
	
	dac_init();
	dac_setMilliVoltage(2500);
	
    while (1) 
    {
		
    }
}
