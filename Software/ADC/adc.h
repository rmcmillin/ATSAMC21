
typedef enum ADC_State{
	ADC_STOPPED,
	ADC_MEASURING,
	ADC_RESULT_READY
}adc_state_t;

adc_state_t adc_getState();

void adc_init();

void adc_triggerMeasurement();

void adc_muxPosSelect(uint8_t posChannel);
void adc_muxNegSelect(uint8_t negChannel);

uint16_t adc_getResult();

void ADC0_Handler();