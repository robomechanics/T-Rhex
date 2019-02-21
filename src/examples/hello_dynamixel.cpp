#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>

#include "dynamixel_sdk.h"

const static std::string device_port_path = "/dev/ttyUSB0";
char dynamixel_ids[] = { 0, 1, 2, 3, 4, 5 };

dynamixel::PortHandler *port_handler = dynamixel::PortHandler::getPortHandler(device_port_path);