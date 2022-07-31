#ifndef ANALOGDATA_H
#define ANALOGDATA_H

#include "warp_protocol.pb.h" //generated using warp_protocol.proto
#include "i2c.h"
#include "gpio.h"

struct ADS_Adc_Data{
	float ch_A0;
	float ch_A1;
	float ch_A2;
	float ch_A3;
};

enum ADC_ControlCommand{
  STOP = 0, // stop sending adc data
  START_CONTINUOUS = 1, // send adc data continuous
  START_ONETIME = 2 // send adc data once
};

extern volatile Sensors sensors;

extern volatile uint8_t cmd_ready_flag;

#define voltage_conversion_constant 0.125 // 2*4096 mV / 2^16 (register size)
#define shunt_amplifier_gain 51
#define shunt_resistance 2 // mOhms
#define voltage_celcius_conversion_constant 10; // 10mV / 1 Celcius

void configure_channel(uint8_t channel, unsigned char* buffer);
void read_channel_data(uint8_t channel, struct ADS_Adc_Data* ADSx, int16_t ADC_Data);
void Read_ADS1115(unsigned char addr, struct ADS_Adc_Data* ADSx);
void AnalogDataRequest(enum ADC_ControlCommand command);
void initialize_ADS_data();

#endif //ANALOGDATA_H