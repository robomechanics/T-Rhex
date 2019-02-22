#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <csignal>

#include "dynamixel_sdk.h"

const static std::string device_port_path = "/dev/ttyUSB0";
const static uint32_t baudrate = 1000000;
const static float protocol_version = 1.0;
const static uint8_t ADDR_MX_GOAL_GET = 0x24;

const static int dynamixel_ids[] = { 0, 1, 2, 3, 4, 6 };
static dynamixel::PortHandler *port_handler;
static dynamixel::PacketHandler *packet_handler;

void signint_handler( int signum )
{
    port_handler->closePort();
    std::cout << "Closing port" << std::endl;
    exit(0);
}

void print_servo_positions(uint16_t *pos, int size)
{
    std::cout << "Servos: \t";
    for (int i = 0; i < size; i++)
    {
        std::cout << i << ":" << pos[i] << "\t";
    }
    std::cout << std::endl;
}

int main()
{
    signal(SIGINT, signint_handler);

    port_handler = dynamixel::PortHandler::getPortHandler(device_port_path.c_str());
    packet_handler = dynamixel::PacketHandler::getPacketHandler(protocol_version);


    if (port_handler->openPort())
    {
        std::cout << "Opened Port" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open Port" << std::endl;
        return 1;
    }

    if(port_handler->setBaudRate(baudrate))
    {
        std::cout << "changed baudrate" << std::endl;
    }
    else
    {
        std::cerr << "failed to change baud rate" << std::endl;
        return 1;
    }

    while (true)
    {
        uint16_t servo_positions[6] = {0,0,0,0,0,0};
        for (int i = 0; i < 6; i++)
        {
            int id = dynamixel_ids[i];
            uint8_t err = 0;
            int dxl_comm_status = packet_handler->read2ByteTxRx(port_handler, id, ADDR_MX_GOAL_GET, &(servo_positions[i]), &err); 

            if (dxl_comm_status != COMM_SUCCESS || err != 0)
            {
                servo_positions[i] = -1;
            }

        }

        print_servo_positions(servo_positions, 6);
    }

    return 0;
}
