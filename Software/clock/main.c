#define F_CPU	32000000UL 
//#define F_CPU	4000000UL

#include "sam.h"
#include "clock.h"
#include "uart.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();	
	//when does this really need to be set??? it works without it
	MCLK->APBAMASK.bit.OSCCTRL_ = 1; //enable bus for osc
		
	OSCCTRL->XOSCCTRL.reg =	OSCCTRL_XOSCCTRL_STARTUP(0x8) | //256 cycle of slow clock ~7816us
							OSCCTRL_XOSCCTRL_AMPGC |		//auto amplitude gain control for crystal
							OSCCTRL_XOSCCTRL_GAIN(0x4) |	//recommended for 32Mhz
							//OSCCTRL_XOSCCTRL_ONDEMAND |	//OSC always on if 0
							//OSCCTRL_XOSCCTRL_RUNSTDBY |	//not running in stdby if 0
							//OSCCTRL_XOSCCTRL_SWBEN |		//switch back to crystal in case of recovery
							//OSCCTRL_XOSCCTRL_CFDEN |		//clock failure detection
							OSCCTRL_XOSCCTRL_XTALEN |		//crystal override gpio pins
							OSCCTRL_XOSCCTRL_ENABLE;		//enable oscillator output
	
	//wait for crystal to be ready
	while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_XOSCRDY));
	
	//set gnerator 1 to crystal
	GCLK->GENCTRL[1].reg =	GCLK_GENCTRL_DIV(0)|
							GCLK_GENCTRL_GENEN|
							GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC_Val);
	
	//32.768khz clock
	MCLK->APBAMASK.reg |= MCLK_APBAMASK_OSC32KCTRL;

	// Select external 32.768kHz crystal oscillator and enable it
	OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_STARTUP(0x4u) |
	OSC32KCTRL_XOSC32K_XTALEN |
	OSC32KCTRL_XOSC32K_EN32K |
	OSC32KCTRL_XOSC32K_ENABLE;
	
	// Wait for the crystal to stabilize
	while (!(OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY));
	
	//setup generator for 32.768khz clock
	GCLK->GENCTRL[2].reg =	GCLK_GENCTRL_DIV(0)|
	GCLK_GENCTRL_GENEN|
	GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K_Val);


		
	
	//RTC Setup
	MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC;
	//reset RTC registers to default (this was really needed as i had to unplug power after programming to get it to work)
	RTC->MODE0.CTRLA.bit.SWRST = 1;
	while (RTC->MODE0.CTRLA.bit.SWRST); //wait for reset
	
	OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K;				
			
	RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_PRESCALER_DIV1|
							RTC_MODE0_CTRLA_MODE_COUNT32|
							RTC_MODE0_CTRLA_MATCHCLR;									
	
	RTC->MODE0.COMP->reg = 32768;
	RTC->MODE0.INTENSET.reg = RTC_MODE0_INTENSET_CMP0;

	RTC->MODE0.CTRLA.bit.ENABLE = 1;

	// Wait for the RTC synchronization
	//while (RTC->MODE0.STATUS.reg & RTC_MODE0_STA);

	NVIC_EnableIRQ(RTC_IRQn);
	
	
	//enable nvic
	__enable_irq();	
	
	uart_init(115200, F_CPU); //uart set to use GCLK[1]
	uart_sendString("osc status register: ");
	printBinaryDword(OSCCTRL->STATUS.reg);
	NEW_LINE
	
	uart_sendString("osc32 status register: ");
	printBinaryDword(OSC32KCTRL->STATUS.reg);
	NEW_LINE

	/* Replace with your application code */
	
	while (1)
	{		
			//printBinaryDword(RTC->MODE0.COUNT.reg);
			//NEW_LINE	
		
	}
}

void RTC_Handler(){
	//clear flag
	//RTC->MODE0.INTFLAG.bit.CMP0 |= 1;
	RTC->MODE0.INTFLAG.reg |= RTC_MODE0_INTFLAG_CMP0;	 
	uart_sendString("i\r\n");
}
