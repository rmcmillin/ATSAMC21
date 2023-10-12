#include "sam.h"
#include "eic.h"
#include "uart.h"


void eic_init(){
	//enable the core clock for sercom3
	GCLK->PCHCTRL[EIC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
	
	//enable the clock for UART
	MCLK->APBAMASK.reg |= MCLK_APBAMASK_EIC;
	
	//enable peripheral mutiplexer for pins PA02
	PORT->Group[0].PINCFG[PIN_PA02].bit.PMUXEN = 1;	
	
	//pins PA02 use peripheral A
	PORT->Group[0].PMUX[1].reg = PORT_PMUX_PMUXE(0x0);
	
	
	//EIC->CTRLA.reg |= EIC_CTRLA_CKSEL; //use CLK_ULP32K if set, GCLK_EIC if not set		
	
	//EIC->ASYNCH.reg |= EIC_ASYNCH_ASYNCH(2);// syncronous edge detection if not set, async if set
	
	EIC->CONFIG[0].reg |= EIC_CONFIG_FILTEN2 | EIC_CONFIG_SENSE2_RISE; //interrupt/event on rising edge of PA02	
	
	EIC->INTENSET.reg |= 1 << PIN_PA02; //enable interrupt on PA02
	
	NVIC_EnableIRQ(EIC_IRQn);	//enable interrupts for sercom3
	
	EIC->CTRLA.reg |= EIC_CTRLA_ENABLE;
	while (EIC->SYNCBUSY.bit.ENABLE); //wait for sync
}

void EIC_Handler(){				
	if (EIC->INTFLAG.reg & EIC_INTFLAG_EXTINT(4)){
		uart_sendString(".");
		EIC->INTFLAG.reg |= EIC_INTFLAG_EXTINT(4); //clear flag		
	}
	//EIC_INTFALG_EXTINT(4) definition is value << pos. this is equivalent of 1<<2 for PIN_PA02
}