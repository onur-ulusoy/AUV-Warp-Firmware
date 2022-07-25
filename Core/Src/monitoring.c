#include "monitoring.h"

void transmitSensorData(UART_HandleTypeDef *huart, Sensors* sensors_data) {
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

#include "ssd1306.h"
#include "fonts.h"

void printSensorData(const Sensors* sensorDataPtr) {
    // Buffer for formatting sensor data
    char OLED_Buffer[128];

    // Clear the display
    SSD1306_Fill(SSD1306_COLOR_BLACK);

    // Print ESC currents
    sprintf(OLED_Buffer, "ESC1: %2.1f A ESC2: %2.1f A ESC3: %2.1f A ESC4: %2.1f A",
            sensorDataPtr->esc_ch0_current,
            sensorDataPtr->esc_ch1_current,
            sensorDataPtr->esc_ch2_current,
            sensorDataPtr->esc_ch3_current);
    SSD1306_GotoXY(0,0);
    SSD1306_Puts(OLED_Buffer, &Font_7x10, SSD1306_COLOR_WHITE);

    sprintf(OLED_Buffer, "ESC5: %2.1f A ESC6: %2.1f A ESC7: %2.1f A ESC8: %2.1f A",
            sensorDataPtr->esc_ch4_current,
            sensorDataPtr->esc_ch5_current,
            sensorDataPtr->esc_ch6_current,
            sensorDataPtr->esc_ch7_current);
    SSD1306_GotoXY(0,15);
    SSD1306_Puts(OLED_Buffer, &Font_7x10, SSD1306_COLOR_WHITE);

    // Print converters data
    sprintf(OLED_Buffer, "CONV1: %2.1f V %2.1f A CONV2: %2.1f V %2.1f A",
            sensorDataPtr->conv1_12V_voltage,
            sensorDataPtr->conv1_12V_current,
            sensorDataPtr->conv2_5VHP_voltage,
            sensorDataPtr->conv2_5VHP_current);
    SSD1306_GotoXY(0,30);
    SSD1306_Puts(OLED_Buffer, &Font_7x10, SSD1306_COLOR_WHITE);

    // Print battery, temperature, and module data
    sprintf(OLED_Buffer, "BATT: %2.1f V ESC TEMP: %2.1f C Module: %2.1f V %2.1f A",
            sensorDataPtr->batt_voltage,
            sensorDataPtr->esc_temp,
            sensorDataPtr->module_voltage,
            sensorDataPtr->module_current);
    SSD1306_GotoXY(0,45);
    SSD1306_Puts(OLED_Buffer, &Font_7x10, SSD1306_COLOR_WHITE);

    // Update the screen with all the new data
    SSD1306_UpdateScreen();
}
