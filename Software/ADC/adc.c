
#include "sam.h"
#include "adc.h"
#include "uart.h"

static uint16_t adc_result;

void adc_init(){
	
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_ADC0; //enable peripheral clock
	
	GCLK->PCHCTRL[ADC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN; //use GLCK0 as clock source			
	
	//setup Pin PA02 as ADC channel 0
	PORT->Group[0].PINCFG[PIN_PA02].bit.PMUXEN = 1;//enable peripheral multiplexer to give peripheral control of pin
	PORT->Group[0].PMUX[1].reg |= PORT_PMUX_PMUXE(0x1); //assign peripheral B to pin 2
	
	ADC0->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV2; //GCLK prescaler
	
	ADC0->REFCTRL.reg |= ADC_REFCTRL_REFSEL_INTVCC2; //use VCC ANA as reference voltage
	
	ADC0->SAMPCTRL.reg = ADC_SAMPCTRL_SAMPLEN(32); //increase sample length for impedance control.  this improved result from 200k -200k voltaeg divider
	
	ADC0->CTRLC.reg |= ADC_CTRLC_RESSEL_12BIT | ADC_CTRLC_FREERUN; //12 bit resolution
	while (ADC0->SYNCBUSY.bit.CTRLC); //wait for clock sync
	
	ADC0->INTENSET.reg |= ADC_INTENSET_RESRDY; //enable interrupt when result is ready				
	
	ADC0->CTRLA.bit.ENABLE = 1;
	while (ADC0->SYNCBUSY.bit.ENABLE); //wait for clock sync
}

void adc_triggerMeasurement(){	
	ADC0->SWTRIG.reg |= ADC_SWTRIG_START;		
}

void adc_muxPosSelect(uint8_t posChannel){
	ADC0->INPUTCTRL.reg |= ADC_INPUTCTRL_MUXPOS(posChannel);
	while (ADC0->SYNCBUSY.bit.INPUTCTRL); //wait for clock sync
}
void adc_muxNegSelect(uint8_t negChannel){
	ADC0->INPUTCTRL.reg |= ADC_INPUTCTRL_MUXNEG(negChannel);
	while (ADC0->SYNCBUSY.bit.INPUTCTRL); //wait for clock sync
}

	
uint16_t adc_getResult(){		
	return ADC0->RESULT.reg;	
}

void ADC0_Handler(){	
	
	if (ADC0->INTFLAG.bit.RESRDY)
	{
		ADC0->INTFLAG.reg |= ADC_INTFLAG_RESRDY; //clear interrupt
		adc_result = ADC0->RESULT.reg;	
	}	
	printWord(adc_result);
	NEW_LINE
}
