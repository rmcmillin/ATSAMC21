#ifndef OSCCLK_H_INCLUDED
#define OSCCLK_H_INCLUDED

//#define F_CPU	32000000UL
//#define F_CPU	40000000UL


/*
OSCCTRL & OSC32KCTRL: select clock source - internal 48MHz, external crystal, digital PLL 96Mhz

GCLK controls the clock distribution
	Generic Clock Generator: programmable prescaler that can use any of the system clock sources as source clock
	Generic Clock Generator 0: GCLK_MAIN - clock that feeds the power manager that generates synchronous clocks
	Generic Clock: uses a generic clock generator to use for a peripheral.  typically multiple instances of a peripheral will use a seperate generic clock
	
Main Clock Controller: MCLK controls the synchronous clocks on the system.

For peripherals, if synchronous clock is used then it's referring to cpu and bus clocks. if asynchronous then it's referring to generic clocks

Clock config for a peripheral
	-first select clock source in OSCCTRL
	-configure GENCTRLn and select it's source (from OSCCTRL)
	-configure PCHCTRLn (peripheral channel control0 and choose the generator to be used
	-unmask the peripheral in MCLK APBxMASK
*/


//OSCILLATOR CONTROL

typedef enum CLKSRC{
	INTERNAL48MHZ,
	EXTERNALOSC,
	DPLL
}clksrc_t;

//select clock source
void oscillatorControl(clksrc_t sourceClock);


typedef enum CLK32SRC{
	INT1KULP,		//1.024 kHz from 32.768 kHz internal ULP oscillator
	INT32KULP,		//32.768 kHz from 32.768 kHz internal ULP oscillator
	INT1K,			//1.024 kHz from 32.768 kHz internal oscillator
	INT32K,			//32.768 kHz from 32.768 kHz internal oscillator
	EXT1KCLK,		//1.024 kHz from 32.768 kHz external oscillator
	EXT32KXTAL		//32.768 kHz from 32.768 kHz external crystal oscillator
}clk32src_t;


//select 32.768khz clock source
void oscillator32Control (clk32src_t sourceClock);



void selectMainClock();



/* Initialize main clock source */
void mclk_init_main_clock(uint8_t src);

/* Initialize MCLK prescaler */
void mclk_init_prescaler(uint8_t div);

#endif /* OSCCLK_H_INCLUDED */