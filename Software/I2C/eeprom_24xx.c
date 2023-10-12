//#include <avr/io.h>
#include "sam.h"
#include "eeprom_24xx.h"
#include "i2c.h"
//#include "UART.h"

//#define F_CPU 2000000UL
//#include <util/delay.h>

uint8_t eeprom_writeByte (uint8_t eepromi2cAddress, uint16_t address, uint8_t byte){
	i2c_start(eepromi2cAddress,I2CWRITE);
	i2c_write(address >> 8); //msb first
	i2c_write(address); //lsb second
	i2c_write(byte);
	i2c_stop();
	//_delay_ms(100);
	return 0;
}

uint8_t eeprom_readByte (uint8_t eepromi2cAddress, uint16_t address, uint8_t *byte){
	i2c_start(eepromi2cAddress,I2CWRITE);
	i2c_write(address >> 8);
	i2c_write(address);
	i2c_start(eepromi2cAddress,I2CREAD);
	*byte = i2c_readNack();
	i2c_stop();
	return 0;
}

uint8_t eeprom_readWord (uint8_t eepromi2cAddress, uint16_t address, uint16_t *word){
	i2c_start(eepromi2cAddress,I2CWRITE);
	i2c_write(address >> 8);
	i2c_write(address);
	i2c_start(eepromi2cAddress,I2CREAD);
	*word = i2c_readAck();
	*word = *word << 8;
	*word |= i2c_readNack();
	i2c_stop();
		
	return 0;
}

uint8_t eeprom_writeWord (uint8_t eepromi2cAddress, uint16_t address, uint16_t word){
	i2c_start(eepromi2cAddress,I2CWRITE);
	i2c_write(address >> 8); //msb first
	i2c_write(address); //lsb second
	i2c_write(word >> 8); //write MSB first
	i2c_write(word);
	i2c_stop();
	//_delay_ms(100);
	return 0;
}

uint8_t eeprom_readDWord (uint8_t eepromi2cAddress, uint16_t address, uint32_t *dword){
	i2c_start(eepromi2cAddress,I2CWRITE);
	i2c_write(address >> 8);
	i2c_write(address);
	i2c_start(eepromi2cAddress,I2CREAD);
	*dword = i2c_readAck();
	*dword = *dword << 8;
	*dword |= i2c_readAck();
	*dword = *dword << 8;
	*dword |= i2c_readAck();
	*dword = *dword << 8;
	*dword |= i2c_readNack();
	i2c_stop();
	
	//printString("Full DWORD in eeprom_readDWord: ",UARTDEBUG);
	//printBinaryDword(*dword);
	//printString("\r\n",UARTDEBUG);
	
	return 0;
}

uint8_t eeprom_writeDWord (uint8_t eepromi2cAddress, uint16_t address, uint32_t dword){
	i2c_start(eepromi2cAddress,I2CWRITE);
	i2c_write(address >> 8); //msb first
	i2c_write(address); //lsb second
	i2c_write((uint8_t)(dword >> 24)); //write MSB first
	i2c_write((uint8_t)(dword >> 16)); 
	i2c_write((uint8_t)(dword >> 8)); 
	i2c_write((uint8_t )dword);
	i2c_stop();
	//_delay_ms(100);
	return 0;
}


uint8_t eeprom_writeMultiByte (uint8_t eepromi2cAddress, uint16_t startAddress, uint8_t *byteArray, uint8_t numBytesToWrite){
	//calculate number of bytes from startAddress to end of Page
	uint8_t remainingPageBytes;
	remainingPageBytes = EEPROM_PAGESIZE - (startAddress % EEPROM_PAGESIZE);
	
	if (numBytesToWrite <= remainingPageBytes){
		//can write in single transaction
		i2c_start(eepromi2cAddress,I2CWRITE);
		i2c_write(startAddress >> 8);
		i2c_write(startAddress);
		for (uint8_t i=0; i<numBytesToWrite; i++){
			i2c_write(byteArray[i]);
		}
		i2c_stop();
		//_delay_ms(100);
	}
	else{
		//crossing page border requires two transactions
		
		//1st transaction
		i2c_start(eepromi2cAddress,I2CWRITE);
		i2c_write(startAddress >> 8);
		i2c_write(startAddress);
		for (uint8_t i=0; i<remainingPageBytes; i++){
			i2c_write(byteArray[i]);
		}
		i2c_stop();
		//_delay_ms(100);
		//2nd transaction
		startAddress += remainingPageBytes;
		i2c_start(eepromi2cAddress,I2CWRITE);
		i2c_write(startAddress >> 8);
		i2c_write(startAddress);
		for (uint8_t i=remainingPageBytes; i<numBytesToWrite; i++){
			i2c_write(byteArray[i]);
		}
		i2c_stop();
	}
	
	return 0;
}

uint8_t eeprom_readMultiByte (uint8_t eepromi2cAddress, uint16_t startAddress, uint8_t *byteArray, uint8_t numBytesToRead){
	//calculate number of bytes from startAddress to end of Page
	uint8_t remainingPageBytes;
	remainingPageBytes = EEPROM_PAGESIZE - (startAddress % EEPROM_PAGESIZE);	
	if (numBytesToRead <= remainingPageBytes){
		//can read in single transaction
		i2c_start(eepromi2cAddress,I2CWRITE);
		i2c_write(startAddress >> 8);
		i2c_write(startAddress);
		i2c_start(eepromi2cAddress,I2CREAD);
		for (uint8_t i=0; i<numBytesToRead; i++){
			byteArray[i] = i2c_readAck();
		}
		i2c_readNack();//dummy byte to send NACK
		i2c_stop();
	}
	else{
		//crossing page border requires two transactions
		
		//1st transaction
		i2c_start(eepromi2cAddress,I2CWRITE);
		i2c_write(startAddress >> 8);
		i2c_write(startAddress);
		i2c_start(eepromi2cAddress,I2CREAD);
		for (uint8_t i=0; i<remainingPageBytes; i++){
			byteArray[i] = i2c_readAck();
		}
		i2c_readNack(); //dummy byte to send NACK
		i2c_stop();
		
		//2nd transaction
		startAddress += remainingPageBytes;
		i2c_start(eepromi2cAddress,I2CWRITE);
		i2c_write(startAddress >> 8);
		i2c_write(startAddress);
		i2c_start(eepromi2cAddress,I2CREAD);
		for (uint8_t i=remainingPageBytes; i<numBytesToRead; i++){
			byteArray[i] = i2c_readAck();
		}
		i2c_readNack(); //dummy byte to send NACK
		i2c_stop();
	}	
	return 0;
}

void eeprom_eraseMemory (uint8_t eepromi2cAddress){
	uint8_t dummyArray [EEPROM_PAGESIZE] = {0xff};
	//init array
	
	for (uint16_t i=0; i<EEPROM_NUMPAGES; i++){
		eeprom_writeMultiByte (eepromi2cAddress, i*EEPROM_PAGESIZE, &dummyArray[0], EEPROM_PAGESIZE);
	}
}

uint8_t eeprom_verifyMemoryErased(uint8_t eepromi2cAddress ){
	return 0;
}

uint8_t eeprom_comparePage (uint8_t *array1, uint8_t *array2, uint8_t arraySize){
	uint8_t result = 1;
	
	for (uint8_t i=0; i<arraySize; i++){
		//printByte(array1[i], UARTDEBUG);
		//printString(" ",UARTDEBUG);
		//printByte(array2[i], UARTDEBUG);
		
		if (array1[i] != array2[i]){
			//printString("no match",UARTDEBUG);
			result = 0;
			//return 0;
		}
		//printString("\r\n",UARTDEBUG);
	}
	
	return result;
}