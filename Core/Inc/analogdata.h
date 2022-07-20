#ifndef ANALOGDATA_H
#define ANALOGDATA_H

#include "proto/warp_protocol.pb.h" //generated using warp_protocol.proto
#include "pb_decode.h"
#include "pb_encode.h"

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

WarpCommand esc_command;
Sensors sensors;

volatile uint8_t cmd_ready_flag = 0;
char OLED_Buffer[70];

#define voltage_conversion_constant 0.125 // 2*4096 mV / 2^16 (register size)
#define shunt_amplifier_gain 51
#define shunt_resistance 2 // mOhms
#define voltage_celcius_conversion_constant 10; // 10mV / 1 Celcius

void configure_channel(uint8_t channel, unsigned char* buffer);
void read_channel_data(uint8_t channel, struct ADS_Adc_Data* ADSx, int16_t ADC_Data);
void Read_ADS1115(unsigned char addr, struct ADS_Adc_Data* ADSx);
void AnalogDataRequest(enum ADC_ControlCommand command);

struct ADS_Adc_Data ADS1;
struct ADS_Adc_Data ADS2;
struct ADS_Adc_Data ADS3;
struct ADS_Adc_Data ADS4;

struct ADS_Adc_Data *ADS_Data[4];

void initialize_ADS_data();

#endif //ANALOGDATA_H