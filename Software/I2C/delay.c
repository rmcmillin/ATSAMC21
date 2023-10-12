#include "sam.h"
#include "delay.h"
#define CPU_FREQUENCY	4000000

void delay_nop(uint32_t delay_cycles) {
	for (volatile uint32_t i = 0; i < delay_cycles; i++) {
		__asm("nop");
	}
}

void delay_ms(uint32_t milliseconds){
	delay_nop(milliseconds*(CPU_FREQUENCY/1000));
}

void delay_us(uint32_t microseconds){
	delay_nop(microseconds*(CPU_FREQUENCY/1000000));
}