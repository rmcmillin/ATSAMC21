#include "sam.h"
#include "mcp3204.h"
#include "spi.h"

#include "uart.h"

uint16_t mcp3024(uint8_t channel){
	uint16_t result = 0;
	uint16_t result_msk = 0b0000111111111111;	
	//select ADC SS
	PORT->Group[0].OUTCLR.reg = PORT_PA07;
	spi_byteSwap(STARTBIT | (channel >> 2) );
	result = spi_byteSwap(channel << 6);	
	result = result << 8;
	result |= spi_byteSwap(DUMMYBYTE);
	
	//deselect ADC
	PORT->Group[0].OUTSET.reg = PORT_PA07;
	
	return (result & result_msk);
}
