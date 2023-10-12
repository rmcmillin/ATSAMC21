#include "sam.h"
#include "clock.h"

void clock_initExternalOsc(){
	
//to change gclk0 clock source, set ondemand=1 in glck0 before changing

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


//set generator 1 to crystal
GCLK->GENCTRL[1].reg =	GCLK_GENCTRL_DIV(0)|
						GCLK_GENCTRL_GENEN|
						GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC_Val);
}

void clock_initExternalOsc32(){
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
}