#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "adc_measurement_task.h"
#include "voltage_sender.h"

#ifdef MCU_ADC
#include "mcu_adc.h"
#elif SPI_ADC
#include "spi_adc.h"
#endif

static uint32_t ticks;

struct settings_t {
   channel_t ch;
   voltage_t voltageType;
   uint32_t  periodMs;
};

//relationship btw adc channel, voltage_type and period
static const struct settings_t settings[] = {
   {ADC_CHAN_0 ,VOLTAGE_A ,100  } ,
   {ADC_CHAN_1 ,VOLTAGE_B ,500  } ,
   {ADC_CHAN_2 ,VOLTAGE_C ,2200 } ,
   {ADC_CHAN_3 ,VOLTAGE_D ,3700 } ,
};

static void coloredPrint(uint8_t i, uint32_t ticks)
{
#define BASE_COLOR 1
   printf("\033[38;5;%um%u @ %08u[ms]\r\n",i+BASE_COLOR,i,ticks);
}

static void incTicks(void)
{
   ticks++;
}

static bool compareTicks(uint32_t periodMs)
{
   return (ticks % periodMs) == 0;
}

//public api
void AdcMeasurementTask_Init(void)
{
   ticks = 0; //set to 1 to avoid capturing all channels @beggining
}

void AdcMeasurementTask ( void ) 
{
   uint8_t i; //255 enought for 4 channels

   for(i = 0; i < (sizeof(settings)/sizeof(settings[0])); i++) {
      if( compareTicks(settings[i].periodMs )) {
         //debug log
         coloredPrint(i,ticks);
         //choose btw adc/spi and get the adc value in the expected format NONE blocking
#ifdef MCU_ADC
         uint16_t adcValue = MCU_ADC_GetAdcData(settings[i].ch);
#elif SPI_ADC
         uint32_t adcValue = SPI_ADC_GetAdcData(settings[i].ch);
#endif
         //send the voltage value to the TCP server NONE blocking
         SendVoltage(settings[i].voltageType ,adcValue);
      }
   }
   incTicks();
};
