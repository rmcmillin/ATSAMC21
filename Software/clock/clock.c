#include <sam.h>
#include "clock.h"


void oscillatorControl(clksrc_t sourceClock){
	//enable osc control module IS THIS NEEDED?
	//MCLK->APBAMASK.reg |= MCLK_APBAMASK_OSCCTRL;
	//check register value to see if it's automatically set
	
	switch (sourceClock){
		case(EXTERNALOSC):{
			//configure external clock
			OSCCTRL->XOSCCTRL.reg =	OSCCTRL_XOSCCTRL_STARTUP(0x8) | //256 cycle of slow clock ~7816us
									OSCCTRL_XOSCCTRL_AMPGC |		//auto amplitude gain control for crystal
									OSCCTRL_XOSCCTRL_GAIN(0x4) |	//recommended for 32Mhz
									//OSCCTRL_XOSCCTRL_ONDEMAND |	//OSC always on if 0
									//OSCCTRL_XOSCCTRL_RUNSTDBY |	//not running in stdby if 0
									//OSCCTRL_XOSCCTRL_SWBEN |		//switch back to crystal in case of recovery
									//OSCCTRL_XOSCCTRL_CFDEN |		//clock failure detection
									OSCCTRL_XOSCCTRL_XTALEN |		//crystal override gpio pins
									OSCCTRL_XOSCCTRL_ENABLE;		//enable oscillator output
									
			//wait for crystal to be ready
			while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_XOSCRDY));	
			/*note
			internal oscillator is enabled by default, but set to on demand only. 48 Div 11+1 = 4Mhz
			*/		
			break;
			}
		case(INTERNAL48MHZ):{
			//configure internal oscillator
			OSCCTRL->OSC48MDIV.reg = OSCCTRL_OSC48MDIV_DIV(0); //48MHz div 0 + 1;
			
			//wait for sync
			while ((OSCCTRL->OSC48MSYNCBUSY.reg & OSCCTRL_OSC48MSYNCBUSY_MASK));
			
			/*note:
			only enable this clock if it has first been completely disabled
			*/
			
			break;
			}
		case(DPLL):{
			/* formula
			dll output = source clock * (LDR + 1 + LDRFRAC/16) * (1/2^pres)
			ie
			50Mhz = 32.000khz * (1562 + 1 + 8/16) * (1/2^0)
			*/
			
			//choose source xosc, x32osc, gclk
			OSCCTRL->DPLLCTRLB.reg |= OSCCTRL_DPLLCTRLB_REFCLK(2);	//0: X32OSC, 1: XOSC, 2: GCLK
			OSCCTRL->DPLLCTRLB.reg |= OSCCTRL_DPLLCTRLB_DIV(0);		//output divider
			//enable DPLL
			OSCCTRL->DPLLCTRLA.reg &= ~OSCCTRL_DPLLCTRLA_ONDEMAND;	//always on
			OSCCTRL->DPLLCTRLA.reg |= OSCCTRL_DPLLCTRLA_ENABLE;		//enable
			//wait for clock to be sync'd
			while (OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_ENABLE);
			
			//set prescaler
			OSCCTRL->DPLLPRESC.reg = OSCCTRL_DPLLPRESC_PRESC(0); //div 1
			//wait for sync
			while (OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_DPLLPRESC);
			
			//set Ratio
			OSCCTRL->DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDR(10) |			//integer part
									OSCCTRL_DPLLRATIO_LDRFRAC(1500);	//fractional part
			//wait for sync
			while (OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_DPLLRATIO);
			
			//wait for clock output to be stable
			while (!(OSCCTRL->DPLLSTATUS.reg & OSCCTRL_DPLLSTATUS_LOCK));
			
			
			break;
			}
	}
}

void oscillator32Control (clk32src_t sourceClock){
	switch (sourceClock){
	case (EXT32KXTAL):{
		// Enable OSC32KCTRL peripheral clock
		//PM->APBAMASK.reg |= PM_APBAMASK_OSC32KCTRL;
		MCLK->APBCMASK.reg |= MCLK_APBAMASK_OSC32KCTRL;

		// Select external 32.768kHz crystal oscillator and enable it
		OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_STARTUP(0x4u) |
		OSC32KCTRL_XOSC32K_XTALEN |
		OSC32KCTRL_XOSC32K_EN32K |
		OSC32KCTRL_XOSC32K_ENABLE;
	 
		// Wait for the crystal to stabilize
		while (!(OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY));
		break;
	}
	case (INT1KULP):{break;}
	case (INT32KULP):{break;}
	case (INT1K):{break;}
	case (INT32K):{break;}
	case (EXT1KCLK):{break;}						
	}
}

void selectMainClock(){
	//gclk_io pin for generator[0] is PA14, which is used by XIN	
	
	//Enable the GCLK module
	//MCLK->APBAMASK.reg |= MCLK_APBAMASK_GCLK;
	//check if automatically set
	
	//create generator[0] for main clock
	GCLK->GENCTRL[0].reg =	GCLK_GENCTRL_DIV(0) |								//clock divider
							//GCLK_GENCTRL_RUNSTDBY |							//0: stopped in standby, 1: runs in standby including gclk_io
							//GCLK_GENCTRL_DIVSEL |								//0: clock div div, 1: clock div 2^(div+1)
							//GCLK_GENCTRL_OE |									//1: output on gclk_io unless selected as a source for another generator
							//GCLK_GENCTRL_OOV |								//0: gclk_io is LOW if generator is off or OE is zero, 1: inverse
							//GCLK_GENCTRL_IDC |								//1: improve duty cycle for gclk_io output to 50%
							GCLK_GENCTRL_GENEN |								//1: enable generator
							GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC);			//generator source clock
							
	//wait for clock to sync							
	while (GCLK->SYNCBUSY.bit.GENCTRL0);
	
	//set cpu divider
	MCLK->CPUDIV.reg = MCLK_CPUDIV_CPUDIV_DIV1;	
	
	//wait for ready (not sure if required, but this bit is set when clock frequency = what it says in CPUDIV)
	while (!MCLK->INTFLAG.bit.CKRDY);
}
