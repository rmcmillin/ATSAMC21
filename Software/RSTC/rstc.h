
typedef enum ResetSource{
	POWER_ON_RESET,
	BODCORE,
	BODVDD,
	UNUSED0,
	EXTERNAL_RESET,
	WATCHDOG,
	SOFTWARE_RESET,
	UNUSED1
}resetSource_t;

resetSource_t rstc_getResetSource();