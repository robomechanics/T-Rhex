#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <stdio.h>
#include <csignal>

#include "dynamixel_sdk.h"
#include "instruction_reader.h"

#ifndef DYN_INTFACE_H
#define DYN_INTFACE_H

const std::string device_port_path = "/dev/ttyUSB0";
const float protocol_version = 1.0;
const uint32_t baudrate = 1000000;

const uint16_t step_speed = 90;
const uint16_t vel_scale = 10;
const uint16_t goal_tolerance = 150;
const uint8_t dynamixel_ids[] = { 0, 1, 2, 3, 4, 6 };
const uint8_t reversal_ids[] = {3, 4, 6};
const uint8_t num_reversal = 3;
const int16_t dynamixel_offsets[] = { 150, 2277, 1069, 855, 211, 339 };

const uint8_t ADDR_MX_TORQUE_ON = 0x18;
const uint8_t ADDR_MX_LED_CTL = 0x19;
const uint8_t ADDR_MX_VEL_SET = 0x20;
const uint8_t ADDR_MX_GET_POS = 0x24;

const uint16_t DYN_ROTATION_TICKS = 4096;

int8_t dyn_intf_init();
int8_t set_dynamixel_positions(const uint8_t id[NUM_DYNAMIXELS], uint16_t goal_position[NUM_DYNAMIXELS], uint16_t goal_velocities[NUM_DYNAMIXELS]);
int8_t dyn_intf_shutdown();


#endif
