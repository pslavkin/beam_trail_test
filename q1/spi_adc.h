#ifndef SPI_ADC_H
#define SPI_ADC_H

typedef enum _channel_t {
   ADC_CHAN_0,
   ADC_CHAN_1,
   ADC_CHAN_2,
   ADC_CHAN_3,
} channel_t;

uint32_t SPI_ADC_GetAdcData(channel_t channel);

#endif
