#define F_CPU	4000000UL

#include "sam.h"
#include "supc.h"
#include "adc.h"
#include "uart.h"

int main(void)
{    
    SystemInit();

	uart_init(115200, F_CPU);
    //reference must be set before enabling ADC
	
	uart_sendString("ADC init");
	NEW_LINE
	NVIC_EnableIRQ(ADC0_IRQn);	//enable interrupts for ADC
	adc_init(); //init ADC
	
	adc_muxPosSelect(ADC_INPUTCTRL_MUXPOS_AIN0_Val); //select analog channel
	//adc_muxPosSelect(ADC_INPUTCTRL_MUXPOS_SCALEDCOREVCC_Val); //select analog channel
	//adc_muxNegSelect(0x18); //select analog channel
	uart_sendString("Channel Select");
	NEW_LINE
	
	
		
	uart_sendString("PUlling Trigger\r\n");
	adc_triggerMeasurement(); //start measurement		
	
	//enable nvic
	__enable_irq();
    while (1) 
    {		
		//ERRATA//
		/*
		once the trigger bit is set, it is stuck at 1.
		ADC can only do freerun, can't retrigger another measurement as it is stuck at 1
		*/
    }
}
