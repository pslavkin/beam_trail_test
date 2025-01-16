SRC = main.c \
	  adc_measurement_task.c \
	  voltage_sender.c \

#take obj from .c files
OBJ = $(SRC:.c=.o)
CC		= gcc
CFLAGS	= -Wall -g
LDFLAGS = -lm


ifeq (MCU, $(ADC_IFACE))
	CFLAGS += -DMCU_ADC
	SRC += mcu_adc.c
	BIN = adcMcu.out	
else ifeq (SPI, $(ADC_IFACE))
	CFLAGS += -DSPI_ADC
	SRC += spi_adc.c
	BIN = adcSpi.out
else
endif


show:
	@echo "Please specify the ADC interface to be used"
	@echo "Usage: make all ADC_IFACE=MCU"
	@echo "Usage: make all ADC_IFACE=SPI"

all:
	@echo "MCU_ADC Variant: $(ADC_IFACE)"
	$(CC) -o $(BIN) $(SRC) $(LDFLAGS) $(CFLAGS)

clean:
	rm  -f *.out $(OBJ)

