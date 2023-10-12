#include "sam.h"
#include "spi.h"

void spi_init(){
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
	PORT->Group[0].PINCFG[18].reg |= PORT_PINCFG_DRVSTR;
	//enable the clock for SPI
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM0;
	
	//set MOSI, SCK, SS as output pins
	PORT->Group[0].DIRSET.reg = PORT_PA08 | PORT_PA09; 		
	PORT->Group[0].DIRSET.reg = PORT_PA07; //ss pin not driven by SPI peripheral
	
	//enable peripheral mutiplexer for pins pa08 - pa11
	PORT->Group[0].PINCFG[PIN_PA08].bit.PMUXEN = 1;
	PORT->Group[0].PINCFG[PIN_PA09].bit.PMUXEN = 1;
	//PORT->Group[0].PINCFG[PIN_PA10].bit.PMUXEN = 1;
	PORT->Group[0].PINCFG[PIN_PA11].bit.PMUXEN = 1;		
	
	//pins pa08, pa09, pa10, pa11 use peripheral C
	PORT->Group[0].PMUX[4].reg = 0x22;
	PORT->Group[0].PMUX[5].reg = PORT_PMUX_PMUXO(0x2);
	
	
	SERCOM0->SPI.CTRLA.reg |=	SERCOM_SPI_CTRLA_MODE(0x03)|	//SPI host mode
								SERCOM_SPI_CTRLA_DIPO(0x03)|	//MISO is pad[3] - PA11 for sercom0
								SERCOM_SPI_CTRLA_DOPO(0x00);	//MOSI is pad[0], SCK is pad[1], SS is pad[2]
								//SERCOM_SPI_CTRLA_CPOL|
								//SERCOM_SPI_CTRLA_CPHA|
								//SERCOM_SPI_CTRLA_DORD;			//LSB transmitted first		
	
	SERCOM0->SPI.CTRLB.reg |=	SERCOM_SPI_CTRLB_CHSIZE(0)|		////8bit transfers
								SERCOM_SPI_CTRLB_MSSEN|			//can only use mssen if there is one client. it also toggles SS every byte
								SERCOM_SPI_CTRLB_RXEN;			//enable the receiver
								
								
	while (SERCOM0->SPI.SYNCBUSY.bit.CTRLB);					//wait for clock sync
	
	SERCOM0->SPI.BAUD.reg =	7;									//BAUD = (4M Mhz / (2*250kHz)) - 1
	
	SERCOM0->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;			//enable spi
	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);					//wait for clock sync
}

void spi_setMode(spi_mode_t mode){
	switch (mode){
		case (MODE0):{
			SERCOM0->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_CPOL;
			SERCOM0->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_CPHA;
			break;
		}
		case (MODE1):{
			SERCOM0->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_CPOL;
			SERCOM0->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_CPHA;
			break;
		}
		case (MODE2):{
			SERCOM0->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_CPOL;
			SERCOM0->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_CPHA;
			break;
		}
		case (MODE3):{
			SERCOM0->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_CPOL;
			SERCOM0->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_CPHA;
			break;
		}
	}
}

uint8_t spi_byteSwap(uint8_t byte){	
	while (!SERCOM0->SPI.INTFLAG.bit.DRE);	//wait for dataready bit to be set (data register is ready for data)
	SERCOM0->SPI.DATA.reg = byte;			//load byte to send
	while (!SERCOM0->SPI.INTFLAG.bit.TXC);	//wait for transfer to complete		
	while (!SERCOM0->SPI.INTFLAG.bit.RXC);	//which one do i wait for	
	return SERCOM0->SPI.DATA.reg;			//return data received
}

spi_error_t spi_transfer (uint8_t *txData, uint8_t *rxData, uint16_t length){	
	return SPI_OK;
}

spi_error_t spi_tx (uint8_t *txData, uint16_t length){
	return SPI_OK;
}

spi_error_t spi_rx (uint8_t dummyByte, uint8_t *rxData, uint16_t length){
	return SPI_OK;
}

/*
void spiInit(){
	//init SPI
	//PORTMUX_TWISPIROUTEA |= PORTMUX_SPI0_ALT1_gc;
	//Configure SPI Output Pins
	PORTA_DIRSET |= 1 << PIN4_bp; //Mosi pin
	PORTA_DIRSET |= 1 << PIN6_bp; //sck
	//since SPI is being used as master, SS pin is unused - disable it
	SPI0_CTRLB |= 1 << SPI_SSD_bp;
	//set as master
	SPI0_CTRLA |= 1 << SPI_MASTER_bp;
	//set prescaler to div4 - 20MHz/64 = 312,500Hz
	SPI0_CTRLA |= SPI_PRESC_DIV64_gc;
	//set SPI mode (This LCD requires mode 3)
	SPI0_CTRLB |= SPI_MODE_0_gc;
	//enable SPI
	SPI0_CTRLA |= 1 << SPI_ENABLE_bp;
}



uint8_t spiByteSwap(uint8_t byte){
	//load byte to transmit to buffer
	SPI0_DATA = byte;
	//wait for transmission to complete
	while((SPI0_INTFLAGS & (1 << SPI_IF_bp))==0){
		//printBinaryByte(SPI0_INTFLAGS);
	}
	
	//return received data
	return SPI0_DATA;
}*/


