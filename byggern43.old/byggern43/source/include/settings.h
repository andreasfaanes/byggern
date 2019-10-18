#define TEST_BIT(reg, bit) (reg & (1 << bit))

#define ADC_MEM_START 0x1400
#define SRAM_MEM_START 0x1800
#define OLED_DATA_MEM_START 0x1200
#define OLED_COM_MEM_START 0x1000

#define JOYCON_MID 127

#define F_CPU 4915200