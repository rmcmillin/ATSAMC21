#define F_CPU 4000000UL
#include "sam.h"
#include "uart.h"
#include "spi.h"
#include "mcp3204.h"
#include  "delay.h"

#include <stdio.h>
#include <string.h>

//#include <stddef.h> // for ptrdiff_t
/*
static char *heap_end = NULL;

void * _sbrk(ptrdiff_t incr)
{
	char *prev_heap_end;

	if (heap_end == NULL) {
		heap_end = (char *)0x20000400; // Adjust this address to match your hardware
	}

	prev_heap_end = heap_end;

	if (heap_end + incr > (char *)0x20008000) { // Adjust this address to match your hardware
		// Heap overflow
		return (void *)-1;
	}

	heap_end += incr;

	return (void *)prev_heap_end;
}
*/


int main(void)
{    
    SystemInit();
	uart_init(115200, F_CPU);
	spi_init();	
	uint16_t adcResult;	
	adcResult = mcp3024(MCP3204_CH_0_SINGLE);		
	
	uart_sendString("ADC Result: ");
	printWord(adcResult);
	uart_sendString(" ");
	printWord((uint32_t)adcResult * 5200UL / 4096UL);
	uart_sendString("mV");
	NEW_LINE
	
	
	//NEW_LINE
	/*
	PORT->Group[0].DIRSET.reg = PORT_PA08 |PORT_PA09 |PORT_PA10|PORT_PA11; 		
	
	
	PORT->Group[0].OUTSET.reg = PORT_PA08;	
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	PORT->Group[0].OUTSET.reg = PORT_PA10;
	PORT->Group[0].OUTSET.reg = PORT_PA11;
	PORT->Group[0].OUTCLR.reg = PORT_PA08;	
	PORT->Group[0].OUTCLR.reg = PORT_PA09;	
	PORT->Group[0].OUTCLR.reg = PORT_PA10;	
	PORT->Group[0].OUTCLR.reg = PORT_PA11;
	*/
	
	//char myString[100];
	//sprintf(myString, "hi %dend\r\n",10);
	//uart_sendString(myString);
	
    while (1) 
    {
    }
}
