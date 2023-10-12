#include "sam.h"
#include "divas.h"


void divas_init(){
	//enable the core clock for divas
	//GCLK->PCHCTRL[DIVAS_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
	
	//enable the clock for DIVAS
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_DIVAS;
	
}

divasError_t divas_uDivision(uint32_t dividend, uint32_t divisor, uint32_t *result, uint32_t *remainder){
	DIVAS->CTRLA.reg = DIVAS_CTRLA_DLZ;
	while (DIVAS->STATUS.bit.BUSY);
	if (dividend == 0){
		return DIVAS_DIVIDE_BY_ZERO;
	}
	DIVAS->DIVIDEND.reg = dividend;
	DIVAS->DIVISOR.reg = divisor;
	
	while (DIVAS->STATUS.bit.BUSY);
	*result = DIVAS->RESULT.reg;
	*remainder = DIVAS->REM.reg;
	
	return DIVAS_OK;
}

divasError_t divas_sDivision(int32_t dividend, int32_t divisor, int32_t *result, int32_t *remainder){
	DIVAS->CTRLA.reg = DIVAS_CTRLA_DLZ | DIVAS_CTRLA_SIGNED;
	while (DIVAS->STATUS.bit.BUSY);
	if (dividend == 0){
		return DIVAS_DIVIDE_BY_ZERO;
	}
	DIVAS->DIVIDEND.reg = dividend;
	DIVAS->DIVISOR.reg = divisor;

	while (DIVAS->STATUS.bit.BUSY);
	*result = DIVAS->RESULT.reg;
	*remainder = DIVAS->REM.reg;

	return DIVAS_OK;
}

divasError_t divas_uSqrt(uint32_t sqrnum, uint32_t *result, uint32_t *remainder){
	DIVAS->CTRLA.reg = DIVAS_CTRLA_DLZ;
	while (DIVAS->STATUS.bit.BUSY);
		
	DIVAS->SQRNUM.reg = sqrnum;
	
	while (DIVAS->STATUS.bit.BUSY);
	*result = DIVAS->RESULT.reg;
	*remainder = DIVAS->REM.reg;
	
	return DIVAS_OK;
}
