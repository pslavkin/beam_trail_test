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
   //just compare using the modulo operator
   return (ticks % periodMs) == 0;
}

//choose btw adc/spi and get the adc value in the expected format NONE blocking
#ifdef MCU_ADC
static uint32_t rawAdcToMv(uint16_t adcValue)
{
   //3.3v reference, 12b resolution and 0-3v input range
   //the requeriments is to measure 3v in mV => (adc * 3300)/2^12
   //to avoid floats I could do adc@16b * 3300@16b fits in 32b
   //and then divide it by 2^12 (shift right 12 positions)
   return ((uint32_t)adcValue * 3300 ) >> 12;
}
static uint32_t readVoltageMv( channel_t ch )
{
   uint16_t rawAdcValue = MCU_ADC_GetAdcData(ch);
   return rawAdcToMv(rawAdcValue);
}

#elif SPI_ADC
static uint32_t rawAdcToMv(uint32_t adcValue)
{
   //5v reference, 32b resolution!!! and 0-3v input range
   //1b= 5v/2^32 = 1.1920928955nV (I'd like to see that in action!)
   //the requeriments is to measure 3v in mV, so a lot of bits could be
   //discarded so I could divide instead of multiply
   //2^32/x = 5000 => x = 2^32/5000 = 858993.4594 ~= 858993 to avoid floats

   return adcValue / 858993;
}
static uint32_t readVoltageMv( channel_t ch )
{
   uint32_t rawAdcValue = SPI_ADC_GetAdcData(ch);
   return rawAdcToMv(rawAdcValue);
}
#endif

//public api
void AdcMeasurementTask_Init(void)
{
   ticks = 0; //set to 1 to avoid capturing all channels @beggining
}

void AdcMeasurementTask ( void ) 
{
   uint8_t  i; //255 enought for 4 channels
   uint32_t voltageMv;

   for(i = 0; i < (sizeof(settings)/sizeof(settings[0])); i++) {
      if( compareTicks(settings[i].periodMs )) {
         //debug log
         coloredPrint(i,ticks);
         //get the voltage converted from adc value
         voltageMv = readVoltageMv(settings[i].ch);
         //send the voltage value to the TCP server NONE blocking
         SendVoltage ( settings[i].voltageType ,voltageMv );
      }
   }
   incTicks();
};
