
typedef enum ReferenceVoltage{
	VREF_1024,
	VREF_2048,
	VREF_4096
}internalVref_t;

void supc_initVref(internalVref_t referenceSelect);