
#include "sam.h"

int main(void)
{
    SystemInit();

	//set pin as output
	PORT->Group[0].DIRSET.reg |= PORT_PA16;
	PORT->Group[0].PINCFG[18].reg |= 1 << 6;
    while (1) 
    {
		//turn off
		PORT->Group[0].OUTSET.reg |= PORT_PA16;
			
		//bad delay - dont use this
		for (uint32_t i = 0; i < 10000; i++)
		asm("nop");
			
		//turn on
		PORT->Group[0].OUTCLR.reg |= PORT_PA16;
    }
}
