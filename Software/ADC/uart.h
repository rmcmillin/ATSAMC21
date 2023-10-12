
#ifndef UART_H
#define UART_H

#define NEW_LINE	uart_sendString("\r\n");

void uart_init(uint32_t baud, uint32_t fcpu);

void uart_sendByte(uint8_t byte);
uint8_t uart_receiveByte();

//must be null terminated
void uart_sendString(const char *myString);

void uart_sendBytes(const char *myString, uint8_t numberOfBytes);

//helper functions
void printBinaryByte(uint8_t byte);
void printHexByte (uint8_t byte);
char nibbleToHexCharacter(uint8_t nibble);
void printBinaryDword(uint32_t dWord);

void printByte(uint8_t byte);
void printWord(uint16_t word);

#endif