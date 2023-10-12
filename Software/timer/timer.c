#include "sam.h"
#include "timer.h"
#include "uart.h"

static uint32_t milliseconds;
static uint32_t seconds;

void timer_init(){
	//set clock source to gclk1
	GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN;
	
	//enable the clock for timer peripheral
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_TC0;
	
	TC0->COUNT32.CTRLA.reg |=	TC_CTRLA_MODE_COUNT32; //32bit count modes
	TC0->COUNT32.WAVE.reg |= TC_WAVE_WAVEGEN_MFRQ;	//match frequency for period
	
	TC0->COUNT32.INTENSET.reg |= TC_INTENSET_MC0; //enable interrupt
	
	TC0->COUNT32.CCBUF[0].reg =32000;
	
	TC0->COUNT32.CTRLA.bit.ENABLE = 1; //enable					
	
	NVIC_EnableIRQ(TC0_IRQn);	//enable interrupts for sercom3
}

uint32_t timer_getMilliseconds(){
	return milliseconds;
}

uint32_t timer_getSeconds(){	
	return seconds;
}

void timer_interruptHandler(){	
	//uart_sendString(".");
	milliseconds++;
	if (milliseconds == 1000){
		milliseconds = 0;
		seconds++;
	}
}

void TC0_Handler(){
	//clear interrupt
	TC0->COUNT32.INTFLAG.reg |= TC_INTFLAG_MC0;
	//run handler
	timer_interruptHandler();
}
