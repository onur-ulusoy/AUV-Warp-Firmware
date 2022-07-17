/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "proto/warp_protocol.pb.h" //generated using warp_protocol.proto
#include "pb_decode.h"
#include "pb_encode.h"
#include "ssd1306.h"
#include "fonts.h"
#include "i2c-lcd.h"

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

struct ADS_Adc_Data{
	float ch_A0;
	float ch_A1;
	float ch_A2;
	float ch_A3;
};

struct ADS_Adc_Data ADS1;
struct ADS_Adc_Data ADS2;
struct ADS_Adc_Data ADS3;
struct ADS_Adc_Data ADS4;


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void AnalogDataRequest(enum ADC_ControlCommand command);
void DriveMotors(const WarpCommand command);
void Read_ADS1115(unsigned char addr, struct ADS_Adc_Data* ADSx);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define UART_MAX_RX_SIZE 64	
#define UART_MAX_TX_SIZE 64

uint8_t uart_rx_buffer[UART_MAX_RX_SIZE];
uint8_t uart_tx_buffer[UART_MAX_TX_SIZE];


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);

  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	
	
	HAL_UART_Receive_IT(&huart1, &uart_rx_buffer[0], UART_MAX_RX_SIZE);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	lcd_init();
	SSD1306_Init();
	SSD1306_Fill((SSD1306_COLOR_t)0);
	
	SSD1306_GotoXY(0,5);
  SSD1306_Puts("ITU AUV Team", &Font_11x18, (SSD1306_COLOR_t) 0x01);
	SSD1306_GotoXY(0,30);
  SSD1306_Puts("Warp Driver Board", &Font_11x18, (SSD1306_COLOR_t) 0x01);
  SSD1306_UpdateScreen();
	HAL_Delay(1000);
	
	SSD1306_Fill((SSD1306_COLOR_t)0);
	HAL_Delay(1000);
	
	SSD1306_GotoXY(0,5);
  SSD1306_Puts("ITU AUV Team", &Font_11x18, (SSD1306_COLOR_t) 0x01);
	SSD1306_GotoXY(0,30);
  SSD1306_Puts("Warp Driver Board", &Font_11x18, (SSD1306_COLOR_t) 0x01);
  SSD1306_UpdateScreen();
	
  while (1)
  {
		if (cmd_ready_flag == 1){
			cmd_ready_flag = 0;
			DriveMotors(esc_command);
		}
		
		AnalogDataRequest(START_CONTINUOUS);
		
		//HAL_Delay(1);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//Global UART Interrupt
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) { // PWM struct is being received from mainboard.
	if (huart->Instance == huart1.Instance) {
		uint32_t rx_count = huart->RxXferCount;
		uint32_t rx_size = huart->RxXferSize;
		
		pb_istream_t pb_instream =  pb_istream_from_buffer(&uart_rx_buffer[0], rx_size);
		pb_decode(&pb_instream, WarpCommand_fields, &esc_command); // PWM struct has being written to the global variable esc_command.
		HAL_UART_Receive_IT(&huart1, &uart_rx_buffer[0], UART_MAX_RX_SIZE); 
		
		cmd_ready_flag = 1; // PWM struct is received successfully. Function DriveMotors will be executed.
	}
}

void DriveMotors(const WarpCommand command) {

  uint16_t pulse;

  const uint16_t ch0_command = command.esc_pwm_ch0;
  pulse = (int) round(1.68 * ch0_command);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);

  const uint16_t ch1_command = command.esc_pwm_ch1;
  pulse = (int) round(1.68 * ch1_command);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);

  const uint16_t ch2_command = command.esc_pwm_ch2;
  pulse = (int) round(1.68 * ch2_command);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse);

  const uint16_t ch3_command = command.esc_pwm_ch3;
  pulse = (int) round(1.68 * ch3_command);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse);

  const uint16_t ch4_command = command.esc_pwm_ch4;
  pulse = (int) round(1.68 * ch4_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pulse);

  const uint16_t ch5_command = command.esc_pwm_ch5;
  pulse = (int) round(1.68 * ch5_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pulse);

  const uint16_t ch6_command = command.esc_pwm_ch6;
  pulse = (int) round(1.68 * ch6_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);

  const uint16_t ch7_command = command.esc_pwm_ch7;
  pulse = (int) round(1.68 * ch7_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, pulse);


}


void Read_ADS1115(unsigned char addr, struct ADS_Adc_Data* ADSx) {
	unsigned char ADS_WriteRead_Buffer[3];
	int16_t ADC_Data;
	unsigned char ADS_Transmit_Addr = addr << 1;
	unsigned char ADS_Receive_Addr = ADS_Transmit_Addr + 1;
	
	for (uint8_t i = 0; i < 4; i++){
		ADS_WriteRead_Buffer[0] = 0x01; //Address point register -> Config = 01
		ADS_WriteRead_Buffer[2] = 0x83; //Config register last 8 bits [7:0] = 10000011 (default) = 0x83
		
		//Config register first 8 bits [15:8]
		switch(i){
			case 0: //ADS ch0
				ADS_WriteRead_Buffer[1] = 0xC3; // 11000011
				break;
			case 1: //ADS ch1
				ADS_WriteRead_Buffer[1] = 0xD3; // 11010011
				break;
			case 2: //ADS ch2
				ADS_WriteRead_Buffer[1] = 0xE3; // 11100011
				break;
			case 3: //ADS ch3
				ADS_WriteRead_Buffer[1] = 0xF3; // 11110011
				break;
			default:
				break;
		}
		
		HAL_I2C_Master_Transmit(&hi2c1, ADS_Transmit_Addr, ADS_WriteRead_Buffer, 3, 100);
		ADS_WriteRead_Buffer[0] = 0x00;
		HAL_I2C_Master_Transmit(&hi2c1, ADS_Transmit_Addr, ADS_WriteRead_Buffer, 1, 100);
		HAL_I2C_Master_Receive(&hi2c1, ADS_Receive_Addr, ADS_WriteRead_Buffer, 2, 100);
		
		ADC_Data = (ADS_WriteRead_Buffer[0] << 8) | ADS_WriteRead_Buffer[1];
		ADC_Data *= voltage_conversion_constant;
		switch(i){
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
		
		
		UART_HandleTypeDef* huart = &huart1;
		uint32_t tx_count = huart->RxXferCount;
		uint32_t tx_size = huart->RxXferSize;
		
		pb_ostream_t pb_ostream =  pb_ostream_from_buffer(&uart_tx_buffer[0], tx_size);
		pb_encode(&pb_ostream, Sensors_fields, &sensors);
	  
		if (HAL_UART_Transmit(&huart1, &uart_tx_buffer[0], UART_MAX_TX_SIZE, 1000)!= HAL_OK) 
			Error_Handler();
		
		SSD1306_Fill((SSD1306_COLOR_t)0);
		
		sprintf(OLED_Buffer, "ESC1: %2.1f A ESC2: %2.1f A ESC3: %2.1f A ESC4: %2.1f A", sensors.esc_ch0_current, sensors.esc_ch1_current, sensors.esc_ch2_current, sensors.esc_ch3_current);
		SSD1306_GotoXY(0,0);
		SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
	  sprintf(OLED_Buffer, "ESC5: %2.1f A ESC6: %2.1f A ESC7: %2.1f A ESC8: %2.1f A", sensors.esc_ch4_current, sensors.esc_ch5_current, sensors.esc_ch6_current, sensors.esc_ch7_current);
		SSD1306_GotoXY(0,15);
		SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
		sprintf(OLED_Buffer, "CONV1: %2.1f V %2.1f A CONV2: %2.1f V %2.1f A", sensors.conv1_12V_voltage, sensors.conv1_12V_current, sensors.conv2_5VHP_voltage, sensors.conv2_5VHP_current);
		SSD1306_GotoXY(0,30);
		SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
		sprintf(OLED_Buffer, "BATT: %2.1f V ESC TEMP: %2.1f C Module: %2.1f V %2.1f A", sensors.batt_voltage, sensors.esc_temp, sensors.module_voltage, sensors.module_current);
		SSD1306_GotoXY(0,45);
		SSD1306_Puts(OLED_Buffer, &Font_7x10, (SSD1306_COLOR_t) 0x01);
		
		
		SSD1306_UpdateScreen();
		
  }
}

void CDC_FS_Receive_CpltCallback(uint8_t *Buf, uint32_t *Len) {
  // Receive Code Here.
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
