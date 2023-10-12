#define I2CREAD		1
#define I2CWRITE	0

typedef enum I2C_Error{
	I2C_OK,
	I2C_TIME_OUT,
	I2C_ARBITRATION_LOST,
	I2C_CLIENT_NO_RESPONSE,
	I2C_BUS_ERROR,
	I2C_BAD_PARAMETER
}i2c_error_t;

typedef enum I2C_Bus_State{
	I2C_BUS_UNKNOWN,
	I2C_BUS_IDLE,
	I2C_BUS_OWNER,
	I2C_BUS_BUSY
}i2c_busState_t;

void i2c_init(uint32_t fcpu);

i2c_error_t i2c_start(uint8_t deviceAddress, uint8_t direction);
void i2c_stop();
void i2c_restart();//uint8_t deviceAddress, uint8_t direction
uint8_t i2c_readAck();
uint8_t i2c_readNack();
void i2c_write(uint8_t byte);

void i2c_restartBus(); // disable i2c, clock out using gpio, restart i2c

i2c_busState_t i2c_getBusState();

i2c_error_t i2c_readBytes(uint8_t deviceAddress, uint8_t registerAddress, uint8_t bytesToRead, uint8_t *data);
i2c_error_t i2c_writeBytes(uint8_t deviceAddress, uint8_t registerAddress, uint8_t bytesToWrite, uint8_t *data);

i2c_error_t i2c_readBytes16(uint8_t deviceAddress, uint16_t registerAddress, uint8_t bytesToRead, uint8_t *data);
i2c_error_t i2c_writeBytes16(uint8_t deviceAddress, uint16_t registerAddress, uint8_t bytesToWrite, uint8_t *data);

i2c_error_t i2c_checkBusError();