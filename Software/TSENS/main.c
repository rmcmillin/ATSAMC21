#define F_CPU	48000000UL

#include "sam.h"
//#include "clock.h"
#include "uart.h"






int main(void)
{
	/* Initialize the SAM system */
	SystemInit();			
	
	// Set flash wait states to maximum for 48 MHz operation
	NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS(2) | NVMCTRL_CTRLB_MANW;

	// Switch to 48MHz clock (disable prescaler)
	OSCCTRL->OSC48MDIV.reg = OSCCTRL_OSC48MDIV_DIV(0);
	while (OSCCTRL->OSC48MSYNCBUSY.reg);
	
	uart_init(115200, F_CPU);	
	
	//TSENS
	
	//need to use undivided internal 48mhz clock 
	
	MCLK->APBAMASK.reg |= MCLK_APBAMASK_TSENS;
	//enable generic clock
	GCLK->PCHCTRL[TSENS_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;		
	
	//from fuses, but should be read from nvm somehow?
	TSENS->GAIN.reg = 0x014FEB;
	TSENS->OFFSET.reg = 0x002C7A;
	TSENS->CAL.reg = 0x1F;
	
	TSENS->CTRLA.bit.ENABLE = 1; //enable
	while (TSENS->SYNCBUSY.bit.ENABLE);
	
	
	
	//enable nvic
	__enable_irq();
	while (1)
	{
	
	//uart_sendString("Start\r\n");
	TSENS->CTRLB.bit.START = 1; //start measurement
	//uart_sendString("Waiting\r\n");
	while (!TSENS->INTFLAG.bit.RESRDY);
	//uart_sendString("Result:");
	uint32_t result = TSENS->VALUE.reg;
	printWord(result);
	uart_sendString(" ");
	
	printHexByte(result >> 16);
	printHexByte(result >> 8);
	printHexByte(result);
	uart_sendString(" ");
	result = ((float)result * 0.0625) - TSENS->OFFSET.reg;
	printHexByte(result >> 8);
	printHexByte(result);
	NEW_LINE
		
	}
}
