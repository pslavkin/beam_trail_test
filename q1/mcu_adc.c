#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcu_adc.h"

uint16_t MCU_ADC_GetAdcData(channel_t channel)
{
   uint16_t value = channel*20+2; //fake value
   //debug print
   printf("     MCU_ADC ch: %u value: %u\r\n",channel, value);
   return value;
}

