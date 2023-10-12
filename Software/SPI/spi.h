
void spi_init();

typedef enum SPI_Mode{
	MODE0,
	MODE1,
	MODE2,
	MODE3
}spi_mode_t;

void spi_setMode(spi_mode_t mode);

uint8_t spi_byteSwap(uint8_t byte);

typedef enum SPI_Error{
	SPI_OK,
	SPI_KO
}spi_error_t;

spi_error_t spi_transfer (uint8_t *txData, uint8_t *rxData, uint16_t length);
spi_error_t spi_tx (uint8_t *txData, uint16_t length);
spi_error_t spi_rx (uint8_t dummyByte, uint8_t *rxData, uint16_t length);