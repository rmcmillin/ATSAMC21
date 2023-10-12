#define EEPROM_I2C_ADDRESS		0b1010000

#define EEPROM_24LC64

#ifdef EEPROM_24LC512
#define EEPROM_MAXBYTES 64000
#define EEPROM_PAGESIZE 128
#define EEPROM_NUMPAGES 500
#define EEPROM_DEFAULTVALUE 0xFF
//#define EEPROM_WASTEDSPACE 63987//63996 //I meant used space //EEPROM_MAXBYTES - (EEPROM_MAXBYTES%EEPROM_PACKETSIZE)
//#define EEPROM_MAX_PACKETS EEPROM_WASTEDSPACE/EEPROM_PACKETSIZE
#endif

#ifdef EEPROM_24LC64
#define EEPROM_MAXBYTES 8000
#define EEPROM_PAGESIZE 32
#define EEPROM_NUMPAGES 250
#define EEPROM_DEFAULTVALUE 0xFF
//#define EEPROM_WASTEDSPACE 7995 //EEPROM_MAXBYTES - (EEPROM_MAXBYTES%EEPROM_PACKETSIZE)
#endif

uint8_t eeprom_writeByte (uint8_t eepromi2cAddress, uint16_t address, uint8_t byte);
uint8_t eeprom_readByte (uint8_t eepromi2cAddress, uint16_t address, uint8_t *byte);

//Stores MSB first
uint8_t eeprom_readWord (uint8_t eepromi2cAddress, uint16_t address, uint16_t *word);
uint8_t eeprom_writeWord (uint8_t eepromi2cAddress, uint16_t address, uint16_t word);

uint8_t eeprom_readDWord (uint8_t eepromi2cAddress, uint16_t address, uint32_t *dword);
uint8_t eeprom_writeDWord (uint8_t eepromi2cAddress, uint16_t address, uint32_t dword);




//use for array of uint8_t
uint8_t eeprom_writeMultiByte (uint8_t eepromi2cAddress, uint16_t startAddress, uint8_t *byteArray, uint8_t numBytesToRead);
uint8_t eeprom_readMultiByte (uint8_t eepromi2cAddress, uint16_t startAddress, uint8_t *byteArray, uint8_t numBytesToWrite);

void eeprom_eraseMemory (uint8_t eepromi2cAddress);
uint8_t eeprom_verifyMemoryErased(uint8_t eepromi2cAddress);

uint8_t eeprom_verifyCRC(uint8_t eepromi2cAddress); //verify contents of all memory

uint8_t eeprom_comparePage (uint8_t *array1, uint8_t *array2, uint8_t arraySize);
