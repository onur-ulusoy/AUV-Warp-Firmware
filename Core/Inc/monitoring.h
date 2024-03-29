#ifndef MONITORING_H
#define MONITORING_H

#include "usart.h"
#include "warp_protocol.pb.h" //generated using warp_protocol.proto
#include "pb_decode.h"
#include "pb_encode.h"
#include "ssd1306.h"
#include "fonts.h"
#include "i2c-lcd.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

void transmitSensorData(UART_HandleTypeDef *huart, Sensors* sensors_data);
void printSensorData(const Sensors* sensorDataPtr);
void printOpeningMessage();
#endif //MONITORING_H