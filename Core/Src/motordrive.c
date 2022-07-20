#include "motordrive.h"

void DriveMotors(const WarpCommand command) {

  uint16_t pulse;

  const uint16_t ch0_command = command.esc_pwm_ch0;
  pulse = (int) round(PULSE_CONSTANT * ch0_command);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);

  const uint16_t ch1_command = command.esc_pwm_ch1;
  pulse = (int) round(PULSE_CONSTANT * ch1_command);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);

  const uint16_t ch2_command = command.esc_pwm_ch2;
  pulse = (int) round(PULSE_CONSTANT * ch2_command);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse);

  const uint16_t ch3_command = command.esc_pwm_ch3;
  pulse = (int) round(PULSE_CONSTANT * ch3_command);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse);

  const uint16_t ch4_command = command.esc_pwm_ch4;
  pulse = (int) round(PULSE_CONSTANT * ch4_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pulse);

  const uint16_t ch5_command = command.esc_pwm_ch5;
  pulse = (int) round(PULSE_CONSTANT * ch5_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pulse);

  const uint16_t ch6_command = command.esc_pwm_ch6;
  pulse = (int) round(PULSE_CONSTANT * ch6_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);

  const uint16_t ch7_command = command.esc_pwm_ch7;
  pulse = (int) round(PULSE_CONSTANT * ch7_command);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, pulse);

}