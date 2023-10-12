#include "sam.h"
#include "ac.h"

void init_ac(){
	
	GCLK->PCHCTRL[AC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN; //use GCLK0 for AC
		
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_AC; //enable the peripheral clock for AC
	
	//enable peripheral mutiplexer for pins pa22 and pa23
	PORT->Group[0].PINCFG[PIN_PA04].bit.PMUXEN = 1; //AIN4
	PORT->Group[0].PINCFG[PIN_PA05].bit.PMUXEN = 1; //AIN5
	PORT->Group[0].PINCFG[PIN_PA18].bit.PMUXEN = 1; //CMP3		
	
	PORT->Group[0].PMUX[2].reg |= PORT_PMUX_PMUXE(0x1) | PORT_PMUX_PMUXO(0x1); //assign peripheral B to AIN4,AIN5
	PORT->Group[0].PMUX[9].reg = PORT_PMUX_PMUXE(0x7); //assign peripheral H to CMP3
			
	
	
	//comparator configuaration
	
	AC->INTENSET.bit.COMP0 = 1; //enable interrupts for comparator0
	
	//AC->SCALER[3].reg = 31; // Vscale = VDD(VALUE+1)/64 = 5(31+1)/64 = 2.5	
	
	AC->COMPCTRL[0].reg |=	AC_COMPCTRL_OUT_ASYNC|	//async output 
							AC_COMPCTRL_FLEN_MAJ5|	//filtering length
							AC_COMPCTRL_HYSTEN|		//enable hysteresis - do not use with low speed(errata)
							AC_COMPCTRL_SPEED_HIGH|	//low speed propagation
							//AC_COMPCTRL_SWAP|		//swap inputs
							AC_COMPCTRL_MUXPOS_PIN0|//AIN0 for comp1/2, AIN4 for comp2/3
							AC_COMPCTRL_MUXNEG_PIN1|//AIN1 for comp1/2, AIN5 for comp2/3
							//AC_COMPCTRL_MUXNEG_VSCALE|
							AC_COMPCTRL_INTSEL_TOGGLE; //interrupt on toggle of cmp output
							//AC_COMPCTRL_SINGLE|		//single shot only mode, otherwise continous
	AC->CTRLA.bit.ENABLE = 1; //enable AC peripheral						
	AC->COMPCTRL[0].bit.ENABLE = 1;
}

