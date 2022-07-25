#include "motordrive.h"

volatile WarpCommand esc_command;

struct ChannelConfig {
    uint16_t command_value;
    TIM_HandleTypeDef* timer;
    uint32_t channel;
};

void DriveMotors(const WarpCommand* command) {
    struct ChannelConfig configs[CHANNEL_COUNT] = {
        {command->esc_pwm_ch0, &htim2, TIM_CHANNEL_1},
        {command->esc_pwm_ch1, &htim2, TIM_CHANNEL_2},
        {command->esc_pwm_ch2, &htim3, TIM_CHANNEL_1},
        {command->esc_pwm_ch3, &htim3, TIM_CHANNEL_2},
        {command->esc_pwm_ch4, &htim4, TIM_CHANNEL_1},
        {command->esc_pwm_ch5, &htim4, TIM_CHANNEL_2},
        {command->esc_pwm_ch6, &htim4, TIM_CHANNEL_3},
        {command->esc_pwm_ch7, &htim4, TIM_CHANNEL_4},
    };

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        uint16_t pulse = (int) round(PULSE_CONSTANT * configs[i].command_value);
        __HAL_TIM_SET_COMPARE(configs[i].timer, configs[i].channel, pulse);
    }
}
