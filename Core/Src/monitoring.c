#include "monitoring.h"

void TransmitSensorData(UART_HandleTypeDef *huart, Sensors* sensors_data) {
    // Check if UART handle and sensor data pointer are not null
    if (huart == NULL || sensors_data == NULL) {
        Error_Handler();
        return;
    }

    // Create an output stream
    uint8_t uart_tx_buffer[Sensors_size];
    pb_ostream_t pb_ostream = pb_ostream_from_buffer(uart_tx_buffer, sizeof(uart_tx_buffer));

    // Encode the sensor data
    bool status = pb_encode(&pb_ostream, Sensors_fields, sensors_data);
    if (!status) {
        Error_Handler();
        return;
    }

    // Transmit the encoded sensor data
    HAL_StatusTypeDef uart_status = HAL_UART_Transmit(huart, uart_tx_buffer, pb_ostream.bytes_written, 1000);
    if (uart_status != HAL_OK) {
        Error_Handler();
    }
}
