#include <cstdint>
#include <string>

#ifndef CONFIG_H
#define CONFIG_H

const uint8_t NUM_LEGS = 6;
const uint8_t NUM_ARMS = 1;
const uint8_t NUM_DYNAMIXELS = NUM_LEGS + NUM_ARMS;
const std::string gait_config_file = "tripod_gait.txt";

#define VERBOSE 0

#endif
