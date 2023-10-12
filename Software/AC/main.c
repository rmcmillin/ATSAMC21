#define F_CPU	4000000UL

#include "sam.h"
#include "ac.h"
#include "uart.h"


int main(void)
{    
    SystemInit();
	
	uart_init(115200, F_CPU);
	
	NVIC_EnableIRQ(AC_IRQn);	//enable interrupts for AC
	init_ac();
	

	//enable nvic
	__enable_irq();
	uart_sendString("start\r\n");
    while (1) 
    {
		//printBinaryByte(AC->CTRLB.bit.START3);
    }
}

void AC_Handler(){	
	if (AC->INTFLAG.bit.COMP0){
		//clear flag
		AC->INTFLAG.reg |= AC_INTFLAG_COMP0;		
		uart_sendString("i");
	}		
}