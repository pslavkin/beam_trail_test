#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "spi_adc.h"

uint32_t SPI_ADC_GetAdcData(channel_t channel)
{
   uint32_t value;
   value = rand() & 0xFFFFFFFF; // 32b adc random values
   value = (value / 5 ) * 3;    // limit to 3v input range
   //debug print
   printf("     SPI_ADC ch: %u value: %10u\r\n",channel, value);
   return value;
}


