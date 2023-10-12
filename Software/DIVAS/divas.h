void divas_init();

typedef enum Divas_Error{
	DIVAS_DIVIDE_BY_ZERO,
	DIVAS_OK	
}divasError_t;

divasError_t divas_uDivision(uint32_t dividend, uint32_t divisor, uint32_t *result, uint32_t *remainder);

divasError_t divas_sDivision(int32_t dividend, int32_t divisor, int32_t *result, int32_t *remainder);

divasError_t divas_uSqrt(uint32_t sqrnum, uint32_t *result, uint32_t *remainder);

