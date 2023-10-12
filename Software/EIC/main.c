#define F_CPU 4000000UL

#include "sam.h"
#include "uart.h"
#include "eic.h"

int main(void)
{
	SystemInit();
	uart_init(115200, F_CPU);
	eic_init();
	__enable_irq();
	while (1)
	{
		//uart_sendString(".");
	}
}
