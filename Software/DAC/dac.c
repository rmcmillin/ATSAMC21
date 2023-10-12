#include "sam.h"
#include "dac.h"

void dac_init(){
	GCLK->PCHCTRL[DAC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN; //use gclk0 for dac clock
		
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_DAC;//enable the clock for DAC
		
	PORT->Group[0].PINCFG[PIN_PA02].bit.PMUXEN = 1;	//enable peripheral mutiplexer for pins PA02 (vout)
	PORT->Group[0].PINCFG[PIN_PA03].bit.PMUXEN = 1;	//enable peripheral mutiplexer for pins PA03 (vrefa)
	
	PORT->Group[0].PMUX[1].reg |= PORT_PMUX_PMUXE(0x1)|PORT_PMUX_PMUXO(0x1); //use peripheral B for PA02, PA03
	
	DAC->CTRLB.reg |= DAC_CTRLB_REFSEL_VREFP | DAC_CTRLB_EOEN; //use vrefa for voltage reference and enable external output
	
	DAC->CTRLA.reg |= DAC_CTRLA_ENABLE; //enable DAC
	
	while (DAC->SYNCBUSY.bit.ENABLE); //wait for clock sync
}

void dac_setMilliVoltage(uint16_t voltage){
	while (DAC->SYNCBUSY.bit.DATA); //wait for previous sync to finish
	//Vout = (DATA/0x3ff)*Vref
	DAC->DATA.reg = ((voltage * 0x3ff) / VOLTAGE_REFERENCE);	
}