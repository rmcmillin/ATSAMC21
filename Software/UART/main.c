#define F_CPU	4000000UL

#include "sam.h"
//#include "clock.h"
#include "uart.h"

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	//MCLK->APBBMASK.reg |= MCLK_APBBMASK_PORT;
	//PORT->Group[0].DIRSET.reg |= PORT_PA28;
	//PORT->Group[0].OUTSET.reg |= PORT_PA28;
	
	// Enable the clock for the Generic Clock Controller (GCLK)
	//MCLK->APBAMASK.reg |= MCLK_APBAMASK_GCLK;
	
	//enable nvic
	__enable_irq();
	
	uart_init(115200, F_CPU);
	uart_sendByte(65);
	uart_sendString("hello");
	NEW_LINE
	printBinaryByte(5);	
	NEW_LINE
	printHexByte(10);
	NEW_LINE
	printBinaryDword(256);
    /* Replace with your application code */
    while (1) 
    {
		
    }
}
