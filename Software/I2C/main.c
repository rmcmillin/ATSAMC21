#define F_CPU 4000000UL

#include "sam.h"
#include "uart.h"
#include "i2c.h"
#include "eeprom_24xx.h"
#include "delay.h"


int main(void)
{   
	
	
    SystemInit();
	uart_init(115200,F_CPU);
	i2c_init(F_CPU);
		
	//WRITE
	uart_sendString("write address 0\r\n");
	i2c_start(EEPROM_I2C_ADDRESS, I2CWRITE);
	i2c_write(0);
	i2c_write(0);
	i2c_write(0);
	i2c_write(1);
	i2c_write(2);
	i2c_write(3);
	i2c_write(4);
	i2c_write(5);
	i2c_write(6);
	i2c_write(7);
	i2c_write(8);
	i2c_write(9);
	i2c_stop();
	// need to wait
	delay_nop(40000);
	
	 //READ		
	uart_sendString("reading register 0\r\n");
	uint8_t byte;	
	i2c_start(EEPROM_I2C_ADDRESS, I2CWRITE);
	i2c_write(0);
	i2c_write(1);
	i2c_start(EEPROM_I2C_ADDRESS, I2CREAD);
	//i2c_restart(EEPROM_I2C_ADDRESS, I2CREAD);
	byte = i2c_readAck();	
	byte = i2c_readAck();	
	byte = i2c_readAck();	
	byte = i2c_readAck();	
	byte = i2c_readAck();	
	byte = i2c_readNack();	
	i2c_stop();
		printByte(byte);
		NEW_LINE
	
	

	//Multibyte
	
	uint8_t data [10] = {1,2,3,4,5,6,7,8,9,10}; 
	i2c_error_t result;
	
	
	result = i2c_writeBytes16(EEPROM_I2C_ADDRESS, 0x00, 10, data);	
	delay_nop(40000); //10ms delay for eeprom to finish write
	
		
	result = i2c_readBytes16(EEPROM_I2C_ADDRESS, 0x00, 10, data);
	if (result == I2C_OK){
		for (uint8_t i=0; i<10; i++){
			printByte(data[i]);			
			NEW_LINE
		}
	}
	else{
		uart_sendString("error: ");
		printByte(result);
		NEW_LINE
	}
	NEW_LINE
	
	
    while (1) 
    {
    }
}

