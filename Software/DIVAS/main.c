#define F_CPU 4000000UL

#include "sam.h"
#include "uart.h"
#include "divas.h"

int main(void)
{    
    SystemInit();
	uart_init(115200, F_CPU);
	
	divas_init();
	uint32_t result;
	uint32_t remainder;
	if (divas_uDivision(123, 7, &result, &remainder) == DIVAS_OK){
		uart_sendString("Result: ");
		printWord(result);
		uart_sendString(" R: ");
		printWord(remainder);
		NEW_LINE
	}
	else{
		uart_sendString("Error: div zero\r\n");
	}
	
	if (divas_uSqrt(67, &result, &remainder) == DIVAS_OK){
		uart_sendString("Result: ");
		printWord(result); //result is sqrnum - result^2
		uart_sendString(" R: ");
		printWord(remainder);
		NEW_LINE
	}
	
    while (1) 
    {
		
    }
}
