#define F_CPU	4000000UL //main gen0

#include "sam.h"
#include "uart.h"
#include "rstc.h"
#include "wdr.h"

int main(void)
{
    SystemInit();
	wdr_init();
	uart_init(115200,F_CPU);
	uart_sendString("Reset Source: 0x");
	printHexByte(rstc_getResetSource());
	NEW_LINE
    /* Replace with your application code */
    while (1) 
    {
		wdr_petDog();		
    }
}
