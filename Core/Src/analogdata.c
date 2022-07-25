#include "analogdata.h"
#include "i2c.h"
volatile Sensors sensors;
volatile uint8_t cmd_ready_flag = 0;

struct ADS_Adc_Data ADS1;
struct ADS_Adc_Data ADS2;
struct ADS_Adc_Data ADS3;
struct ADS_Adc_Data ADS4;

struct ADS_Adc_Data *ADS_Data[4];

void configure_channel(uint8_t channel, unsigned char* buffer) {
    buffer[0] = 0x01; //Address point register -> Config = 01
    buffer[2] = 0x83; //Config register last 8 bits [7:0] = 10000011 (default) = 0x83
    
    switch(channel){
        case 0: //ADS ch0
            buffer[1] = 0xC3; // 11000011
            break;
        case 1: //ADS ch1
            buffer[1] = 0xD3; // 11010011
            break;
        case 2: //ADS ch2
            buffer[1] = 0xE3; // 11100011
            break;
        case 3: //ADS ch3
            buffer[1] = 0xF3; // 11110011
            break;
        default:
            break;
    }
}

void read_channel_data(uint8_t channel, struct ADS_Adc_Data* ADSx, int16_t ADC_Data) {
    ADC_Data *= voltage_conversion_constant;
    
    switch(channel){
        case 0: //ADS ch0
            ADSx->ch_A0 = ADC_Data;
            break;
        case 1: //ADS ch1
            ADSx->ch_A1 = ADC_Data;
            break;
        case 2: //ADS ch2
            ADSx->ch_A2 = ADC_Data;
            break;
        case 3: //ADS ch3
            ADSx->ch_A3 = ADC_Data;
            break;
        default:
            break;
    }
}

void Read_ADS1115(unsigned char addr, struct ADS_Adc_Data* ADSx) {
    unsigned char ADS_WriteRead_Buffer[3];
    int16_t ADC_Data;
    unsigned char ADS_Transmit_Addr = addr << 1;
    unsigned char ADS_Receive_Addr = ADS_Transmit_Addr + 1;
    
    for (uint8_t i = 0; i < 4; i++){
        configure_channel(i, ADS_WriteRead_Buffer);
        
        HAL_I2C_Master_Transmit(&hi2c1, ADS_Transmit_Addr, ADS_WriteRead_Buffer, 3, 100);
        ADS_WriteRead_Buffer[0] = 0x00;
        HAL_I2C_Master_Transmit(&hi2c1, ADS_Transmit_Addr, ADS_WriteRead_Buffer, 1, 100);
        HAL_I2C_Master_Receive(&hi2c1, ADS_Receive_Addr, ADS_WriteRead_Buffer, 2, 100);
        
        ADC_Data = (ADS_WriteRead_Buffer[0] << 8) | ADS_WriteRead_Buffer[1];
        
        read_channel_data(i, ADSx, ADC_Data);
    }
    HAL_Delay(10);
}

void AnalogDataRequest(enum ADC_ControlCommand command) {
  if (command == START_ONETIME) {
																																						 //Trying to calculate:
		Read_ADS1115(0x48, &ADS1); //Addr to GND -> Slave Address = 1001000 = 0x48 ESC Curr Set1
		Read_ADS1115(0x49, &ADS2); //Addr to VDD -> Slave Address = 1001001 = 0x49 ESC Curr Set2
		Read_ADS1115(0x4A, &ADS3); //Addr to SDA -> Slave Address = 1001001 = 0x4A 12V,5VHP,BATT,ESC_TEMP Sense
		Read_ADS1115(0x4B, &ADS4); //Addr to SCL -> Slave Address = 1001011 = 0x4B 12V Curr, 5V Curr, Module Curr, Module Voltage

		//ESC shunt																																				//Ex: 2800mV / 51 / 2mOhms = 27.45 Amperes
		sensors.esc_ch0_current = (ADS1.ch_A0 / shunt_amplifier_gain) / shunt_resistance; //ESC 1
		sensors.esc_ch1_current = (ADS1.ch_A1 / shunt_amplifier_gain) / shunt_resistance; //ESC 2
		sensors.esc_ch2_current = (ADS1.ch_A2 / shunt_amplifier_gain) / shunt_resistance; //ESC 3
		sensors.esc_ch3_current = (ADS1.ch_A3 / shunt_amplifier_gain) / shunt_resistance; //ESC 4
		sensors.esc_ch4_current = (ADS2.ch_A0 / shunt_amplifier_gain) / shunt_resistance; //ESC 5
		sensors.esc_ch5_current = (ADS2.ch_A3 / shunt_amplifier_gain) / shunt_resistance; //ESC 6
		sensors.esc_ch6_current = (ADS2.ch_A1 / shunt_amplifier_gain) / shunt_resistance; //ESC 7
		sensors.esc_ch7_current = (ADS2.ch_A2 / shunt_amplifier_gain) / shunt_resistance; //ESC 8

		//CONVERTER 1
		sensors.conv1_12V_voltage = ADS3.ch_A0;
		sensors.conv1_12V_current = (ADS4.ch_A0 / shunt_amplifier_gain) / shunt_resistance;
		
		//CONVERTER 2
		sensors.conv2_5VHP_voltage = ADS3.ch_A1;
		sensors.conv2_5VHP_current = (ADS4.ch_A1 / shunt_amplifier_gain) / shunt_resistance;
		
		//BATT
		sensors.batt_voltage = ADS3.ch_A2;

		//TEMPERATURE SENSOR
		sensors.esc_temp = ADS3.ch_A3 / voltage_celcius_conversion_constant;
		
		//CURRENT-VOLTAGE MODULE
		sensors.module_voltage = ADS4.ch_A2;
		sensors.module_current = ADS4.ch_A3; // !!NOT COMPLETED!! // Shunt resistance? Amplifier gain?
  }
}

void initialize_ADS_data()
{
    // Initialize ADS1, ADS2, ADS3, ADS4

    ADS_Data[0] = &ADS1;
    ADS_Data[1] = &ADS2;
    ADS_Data[2] = &ADS3;
    ADS_Data[3] = &ADS4;
}
