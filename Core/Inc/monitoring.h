#ifndef MONITORING_H
#define MONITORING_H

#include "usart.h"
#include "warp_protocol.pb.h" //generated using warp_protocol.proto
#include "pb_decode.h"

void TransmitSensorData(UART_HandleTypeDef *huart, Sensors* sensors_data);

#endif MONITORING_H