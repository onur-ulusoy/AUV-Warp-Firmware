#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#include "proto/warp_protocol.pb.h" //generated using warp_protocol.proto
#include "tim.h"

#define PULSE_CONSTANT 1.68
#define CHANNEL_COUNT 8

WarpCommand esc_command;

void DriveMotors(const WarpCommand command);

#endif //MOTOR_DRIVE_H