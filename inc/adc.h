
/*
code lifted from
https://binaryupdates.com/adc-in-avr-atmega32a-microcontroller/
*/

#ifndef __ADC_H__
#define __ADC_H__

void adc_init(void);
uint16_t read_adc(uint8_t channel);

#endif /* __ADC_H__ */