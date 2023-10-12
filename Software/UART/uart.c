#include "sam.h"
#include "uart.h"

void uart_init(uint32_t baud, uint32_t fcpu){	
	
	//this can use an another clock generator, or use gen0 main
	//configure generator 1 for sercom3
	//GCLK->GENCTRL[1].reg = 0x106;
		
	//enable the core clock for sercom3	
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
				
	//enable the clock for UART
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM3;				
	
	//enable peripheral mutiplexer for pins pa22 and pa23	
	PORT->Group[0].PINCFG[PIN_PA22].bit.PMUXEN = 1;
	PORT->Group[0].PINCFG[PIN_PA23].bit.PMUXEN = 1;
	
	//pins PA22, PA23 use peripheral C
	PORT->Group[0].PMUX[11].reg = 0x22; 
	
	SERCOM3->USART.CTRLA.reg |=	SERCOM_USART_CTRLA_DORD|		//LSB first
								SERCOM_USART_CTRLA_MODE(0x1)|	//use internal clock for shifting bits
								SERCOM_USART_CTRLA_RXPO(1)|		//use pad0 for rx
								SERCOM_USART_CTRLA_TXPO(0);		//use pad1 for tx, xck pin NC																			
								
	SERCOM3->USART.CTRLB.reg |=	SERCOM_USART_CTRLB_CHSIZE(0)|	//8 data bits
								SERCOM_USART_CTRLB_TXEN|		//enable transmitter
								SERCOM_USART_CTRLB_RXEN;		//enable receiver
	
	SERCOM3->USART.INTENSET.reg |= SERCOM_USART_INTENSET_RXC;	//enable interrupt on byte received
							
	SERCOM3->USART.BAUD.reg = 65536 - ((65536 * 16.0f * baud) / fcpu); //baud calculation
	
	SERCOM3->USART.CTRLA.bit.ENABLE = 1;	//enable peripheral
		
	NVIC_EnableIRQ(SERCOM3_IRQn);	//enable interrupts for sercom3
}

void uart_sendByte(uint8_t byte){
	//receive shares the same register, need to check if byte rx'd
	while (!SERCOM3->USART.INTFLAG.bit.DRE);
	SERCOM3->USART.DATA.reg = byte;
}

uint8_t uart_receiveByte(){
	return SERCOM3->USART.DATA.reg;
}


void uart_sendString(const char *myString){
	while (*myString) {
		uart_sendByte(*myString);
		myString++;
	}
}
void uart_sendBytes(const char *myString, uint8_t numberOfBytes){}

//interrupt handler
void SERCOM3_Handler(){
	uint8_t rxByte;	
	if (SERCOM3->USART.INTFLAG.bit.RXC){
		//rxByte = SERCOM3->USART.DATA.reg;
		rxByte = uart_receiveByte();
		uart_sendByte(rxByte); //echo
	}
}

//helper functions	

int is_bit_set(unsigned char byte, unsigned char bit) {
	return (byte & (1 << bit)) != 0;
}

void printBinaryByte(uint8_t byte) {
	uint8_t bit;
	for (bit = 7; bit < 255; bit--) {
		if (is_bit_set(byte, bit))
		uart_sendByte('1');
		else
		uart_sendByte('0');
	}
}

void printHexByte (uint8_t byte){
	uint8_t nibble;
	nibble = (byte & 0b11110000) >> 4;
	uart_sendByte(nibbleToHexCharacter(nibble) );
	nibble = byte & 0b00001111;
	uart_sendByte(nibbleToHexCharacter(nibble));
}


char nibbleToHexCharacter(uint8_t nibble) {	
	if (nibble < 10) {
		return ('0' + nibble);
	}
	else {
		return ('A' + nibble - 10);
	}
}

void printBinaryDword(uint32_t dWord){
	uint8_t bit;
	for (bit = 31; bit < 255; bit--) {
		//if (bit_is_set(byte, bit))
		if (((dWord >> bit) & 1) == 1) {
			uart_sendByte('1');
		}
		else {
			uart_sendByte('0');
		}
		if (bit % 8 == 0){
			uart_sendByte(32); //space
		}
	}	
}
