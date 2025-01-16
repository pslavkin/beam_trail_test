#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcu_adc.h"

uint16_t MCU_ADC_GetAdcData(channel_t channel)
{
   uint16_t value =  rand() & 0x0FFF ; //12b adc random values
   //debug print
   printf("     MCU_ADC ch: %u value: %05u\r\n",channel, value);
   return value;
}

