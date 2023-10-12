#define F_CPU_MAIN	4000000UL //main gen0
#define F_CPU		32000000UL //gen1 

#include "sam.h"
#include "uart.h"
#include "clock.h"
#include "timer.h"

int main(void)
{    
    SystemInit();
	clock_initExternalOsc();    		
	uart_init(115200, F_CPU); 
	
	timer_init();
	
	//enable nvic
	__enable_irq();
	
	uint32_t currentTime;
	uint32_t oldTime = 0;
	
    while (1) 
    {
		
		currentTime = timer_getSeconds();		
		if (currentTime != oldTime){
			printWord(currentTime);
			NEW_LINE
			oldTime = currentTime;			
		}		
    }
}

