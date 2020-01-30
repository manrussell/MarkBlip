
/*
code lifted from
https://binaryupdates.com/adc-in-avr-atmega32a-microcontroller/
*/

#ifndef __ADC_H__
#define __ADC_H__

void 	 ADC_init(void);
uint16_t ADC_read(uint8_t channel);
void 	 ADC_tx_all_adc_pot_data( void );

#endif /* __ADC_H__ */