#include "sam.h"
#include "i2c.h"
#include "uart.h"
#include "delay.h"

#define I2CREAD		1
#define I2CWRITE	0



static uint32_t cpuFreq; //store passed cpu in case of bus reset

void i2c_init(uint32_t fcpu){
	cpuFreq = fcpu;
	
	//enable the core clock for sercom0
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
	
	//enable the clock for sercom0 for i2c
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM0;
	
	//set pins as outputs
	PORT->Group[0].DIRSET.reg |= PORT_PA08 | PORT_PA09; 
	
	//enable peripheral mutiplexer for pins PA08 and PA09
	PORT->Group[0].PINCFG[PIN_PA08].bit.PMUXEN = 1;
	PORT->Group[0].PINCFG[PIN_PA09].bit.PMUXEN = 1;
	
	//pins PA08, PA09 use peripheral C
	PORT->Group[0].PMUX[4].reg = 0x22;
	
	SERCOM0->I2CM.CTRLA.reg |=	SERCOM_I2CM_CTRLA_MODE(0x5); //set master mode
	
	//SERCOM0->I2CM.CTRLB.bit.SMEN = 1; //enable smart mode (send ackact after reading data)
	
	SERCOM0->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_INACTOUT(0x3);	//205us time out, no timeout not allowed - not in errata
								
	SERCOM0->I2CM.BAUD.reg = 20; //100khz at 4Mhz clock											
															
	SERCOM0->I2CM.CTRLA.bit.ENABLE = 1; //enable i2c								
	while(SERCOM0->I2CM.SYNCBUSY.bit.ENABLE);
	
	SERCOM0->I2CM.STATUS.reg = 0b01; //force bus state to idle
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP);
}

i2c_error_t i2c_start(uint8_t deviceAddress, uint8_t direction){
	//check if bus idle???
		
	SERCOM0->I2CM.ADDR.reg = (deviceAddress << 1 | direction);
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP);			
	
	if (direction == I2CREAD){	
		while (!SERCOM0->I2CM.INTFLAG.bit.SB){
			//check for errors hile waiting for SB bit to be set						
			if (SERCOM0->I2CM.INTFLAG.bit.ERROR){
				return I2C_BUS_ERROR;
			}
		};
		//clear flag
		//SERCOM0->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_SB; 		
	}	
	else if (direction ==I2CWRITE){
		while (!SERCOM0->I2CM.INTFLAG.bit.MB){
			if (SERCOM0->I2CM.INTFLAG.bit.ERROR){
				return I2C_BUS_ERROR;
			}
		}
		SERCOM0->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB; //clear flag		
	}
	else{
		return I2C_BAD_PARAMETER;		
	}
	
	//catch errors, clear error flag and return from function
	
	if (SERCOM0->I2CM.STATUS.bit.BUSERR == 1){		
		SERCOM0->I2CM.STATUS.bit.BUSERR = SERCOM_I2CM_STATUS_BUSERR;
		return I2C_BUS_ERROR;
	}
	else if (SERCOM0->I2CM.STATUS.bit.ARBLOST == 1){		
		SERCOM0->I2CM.STATUS.reg |= SERCOM_I2CM_STATUS_ARBLOST;
		return I2C_ARBITRATION_LOST;
	}
	else if (SERCOM0->I2CM.STATUS.bit.RXNACK == 1){
		SERCOM0->I2CM.STATUS.reg |= SERCOM_I2CM_STATUS_RXNACK;
		return I2C_CLIENT_NO_RESPONSE;
	}
	
	return I2C_OK;
}

void i2c_stop(){
	SERCOM0->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);  // Send STOP condition
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP); //wait for stop
}

void i2c_restart(){
	SERCOM0->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(0x1);
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP); //wait for restart
}

uint8_t i2c_readAck(){
	
	uint8_t data = SERCOM0->I2CM.DATA.reg;	
	SERCOM0->I2CM.CTRLB.reg &= ~(SERCOM_I2CM_CTRLB_ACKACT); //send ACK after reading data
	SERCOM0->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(0x2);
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP); //wait for restart
	SERCOM0->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_SB;// clear flag
	delay_nop(40); //0.00001s delay @4Mhz
	//sequential read failed here without delay. possibly need client clock stretching or wait for some (unknown) bit to set
	//interesting that the minimum delay was 10us or 1/100khz
	//possible clock sync issue
	//using smart mode instead of the cmd register may help, but it has other issues.
	return data;
}

uint8_t i2c_readNack(){	
	uint8_t data = SERCOM0->I2CM.DATA.reg;
	SERCOM0->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT; //send Nack		should this be beofre register read?
	SERCOM0->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(0x3);
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP); //wait for restart
	SERCOM0->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_SB;// clear flag
	
	//wait?	 
	return data;
}

void i2c_write(uint8_t byte){
	
	SERCOM0->I2CM.DATA.reg = byte;  // Write the byte to the DATA register
	while (SERCOM0->I2CM.SYNCBUSY.bit.SYSOP);
	
	while (!SERCOM0->I2CM.INTFLAG.bit.MB){
		if (i2c_checkBusError() == I2C_BUS_ERROR){	
			break;
		}
	}  // Wait for byte to be sent	
}

void i2c_restartBus(){
	SERCOM0->I2CM.CTRLA.bit.SWRST = 1;
	while (SERCOM0->I2CM.SYNCBUSY.bit.SWRST);
	i2c_init(cpuFreq);
}

i2c_busState_t i2c_getBusState(){
	switch ((SERCOM0->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE_Msk) >> SERCOM_I2CM_STATUS_BUSSTATE_Pos){
		case (0):{
			return I2C_BUS_UNKNOWN;
			break;
		}
		case (1):{
			return I2C_BUS_IDLE;
			break;
		}
		case (2):{
			return I2C_BUS_OWNER;
			break;
		}
		case (3):{
			return I2C_BUS_BUSY;
			break;
		}
	}
	
	return I2C_BUS_UNKNOWN;
}

i2c_error_t i2c_checkBusError(){
	if (SERCOM0->I2CM.INTFLAG.bit.ERROR){
		uart_sendString("Bus error - Bus State: ");
		printBinaryByte(i2c_getBusState());
		NEW_LINE		
		return I2C_BUS_ERROR;
	}
	return I2C_OK;
}

i2c_error_t i2c_readBytes(uint8_t deviceAddress, uint8_t registerAddress, uint8_t bytesToRead, uint8_t *data){
		
	i2c_error_t startFailure;
	if ((startFailure = i2c_start(deviceAddress, I2CWRITE))){
		return startFailure;
	}
	
	i2c_write(registerAddress);
	
	i2c_start(deviceAddress, I2CREAD);
	
	for (uint8_t i=0; i<bytesToRead-1; i++){
		*data = i2c_readAck();
		data++;
	}
	*data = i2c_readNack();
	
	i2c_stop();
	
	return I2C_OK;
}

i2c_error_t i2c_writeBytes(uint8_t deviceAddress, uint8_t registerAddress, uint8_t bytesToWrite, uint8_t *data){
	i2c_error_t startFailure;
	if ((startFailure = i2c_start(deviceAddress, I2CWRITE))){
		return startFailure;
	}
	
	i2c_write(registerAddress);
	
	for (uint8_t i=0; i<bytesToWrite; i++){
		i2c_write(*data);
		data++;
	}	
	
	i2c_stop();
	
	return I2C_OK;
}

i2c_error_t i2c_readBytes16(uint8_t deviceAddress, uint16_t registerAddress, uint8_t bytesToRead, uint8_t *data){
	i2c_error_t startFailure;	
	if ((startFailure = i2c_start(deviceAddress, I2CWRITE))){		
		return startFailure;
	}	
	i2c_write(registerAddress);
	i2c_write(registerAddress >> 8);
		
	i2c_start(deviceAddress, I2CREAD);	
	for (uint8_t i=0; i<bytesToRead-1; i++){		
		*data = i2c_readAck();
		data++;
	}	
	*data = i2c_readNack();	
	i2c_stop();	
	
	return I2C_OK;
}

i2c_error_t i2c_writeBytes16(uint8_t deviceAddress, uint16_t registerAddress, uint8_t bytesToWrite, uint8_t *data){
	i2c_error_t startFailure;
	if ((startFailure = i2c_start(deviceAddress, I2CWRITE))){
		return startFailure;
	}
	
	i2c_write(registerAddress);
	i2c_write(registerAddress >> 8);		
	
	for (uint8_t i=0; i<bytesToWrite; i++){
		i2c_write(*data);
		data++;
	}
	
	i2c_stop();
	
	return I2C_OK;
}

