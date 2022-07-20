#include "analogdata.h"
#include "i2c.h"

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
		
		
	// 	UART_HandleTypeDef* huart = &huart1;
	// 	uint32_t tx_count = huart->RxXferCount;
	// 	uint32_t tx_size = huart->RxXferSize;
		
	// 	pb_ostream_t pb_ostream =  pb_ostream_from_buffer(&uart_tx_buffer[0], tx_size);
	// 	pb_encode(&pb_ostream, Sensors_fields, &sensors);
	  
	// 	if (HAL_UART_Transmit(&huart1, &uart_tx_buffer[0], UART_MAX_TX_SIZE, 1000)!= HAL_OK) 
	// 		Error_Handler();
		
	// 	SSD1306_Fill((SSD1306_COLOR_t)0);
		
	// 	sprintf(OLED_Buffer, "ESC1: %2.1f A ESC2: %2.1f A ESC3: %2.1f A ESC4: %2.1f A", sensors.esc_ch0_current, sensors.esc_ch1_current, sensors.esc_ch2_current, sensors.esc_ch3_current);
	// 	SSD1306_GotoXY(0,0);
	// 	SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
	//   sprintf(OLED_Buffer, "ESC5: %2.1f A ESC6: %2.1f A ESC7: %2.1f A ESC8: %2.1f A", sensors.esc_ch4_current, sensors.esc_ch5_current, sensors.esc_ch6_current, sensors.esc_ch7_current);
	// 	SSD1306_GotoXY(0,15);
	// 	SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
	// 	sprintf(OLED_Buffer, "CONV1: %2.1f V %2.1f A CONV2: %2.1f V %2.1f A", sensors.conv1_12V_voltage, sensors.conv1_12V_current, sensors.conv2_5VHP_voltage, sensors.conv2_5VHP_current);
	// 	SSD1306_GotoXY(0,30);
	// 	SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
	// 	sprintf(OLED_Buffer, "BATT: %2.1f V ESC TEMP: %2.1f C Module: %2.1f V %2.1f A", sensors.batt_voltage, sensors.esc_temp, sensors.module_voltage, sensors.module_current);
	// 	SSD1306_GotoXY(0,45);
	// 	SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
		
	// 	SSD1306_UpdateScreen();
		
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
