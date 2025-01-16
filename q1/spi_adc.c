#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "spi_adc.h"

uint32_t SPI_ADC_GetAdcData(channel_t channel)
{
   uint32_t value = channel*10+1; //fake value
   //debug print
   printf("     SPI_ADC ch: %u value: %u\r\n",channel, value);
   return value;
}


