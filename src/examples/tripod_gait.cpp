// pre-down pos (3,4,6) = 3800
// pre-dowm pos (0,1,2) = 3300
// post-down pos (3,4,6) = 3300
// post-down pos (0,1,2) = 3800
// pre-up pos (3,4,6) = 2000
// pre-up pos (0,1,2) = 1200
// post-up pos (3,4,6) = 1200
// post-up pos (0,1,2) = 2000

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
const static uint8_t ADDR_MX_TORQUE_ENABLE = 0x18;
const static uint8_t ADDR_MX_LED_CTL = 0x19;
const static uint8_t ADDR_MX_GOAL_SET = 0x1E;

static dynamixel::PortHandler *port_handler;
static dynamixel::PacketHandler *packet_handler;
const static int dynamixel_ids[] = { 0, 1, 2, 3, 4, 6 };

static const uint16_t pre_down_346 = 4000;
static const uint16_t pre_down_012 = 3000;
static const uint16_t post_down_346 = 3000;
static const uint16_t post_down_012 = 4000;
static const uint16_t pre_up_346 = 2000;
static const uint16_t pre_up_012 = 1000;
static const uint16_t post_up_346 = 1000;
static const uint16_t post_up_012 = 2000;

void signint_handler( int signum )
{
    for (int id : dynamixel_ids)
    {
        uint8_t dxl_err = 0;
        int16_t dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 0, &dxl_err);
    }
    port_handler->closePort();
    std::cout << "Closing port" << std::endl;
    exit(0);
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

    for (int id : dynamixel_ids)
    {
        uint8_t dxl_err = 0;
        int16_t dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 1, &dxl_err);

        if (dxl_comm_res != COMM_SUCCESS || dxl_err)
        {
            std::cerr << "error: " << id << ":" << dxl_comm_res << ", " << packet_handler->getRxPacketError(dxl_err) << std::endl;
        }
        usleep(100000);
    }

    uint16_t goal = 0;

    while (true)
    {
        int16_t dxl_comm_res;
        uint8_t dxl_err;
        /*
        // set pre-down for 1,3,6
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 1, ADDR_MX_GOAL_SET, pre_down_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 3, ADDR_MX_GOAL_SET, pre_down_346, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 6, ADDR_MX_GOAL_SET, pre_down_346, &dxl_err);
        // set pre-up for 0,2,4
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 0, ADDR_MX_GOAL_SET, pre_up_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 2, ADDR_MX_GOAL_SET, pre_up_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 4, ADDR_MX_GOAL_SET, pre_up_346, &dxl_err);

        // pause for 0.25s
        usleep(1000000);

        // set post-down for 1,3,6
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 1, ADDR_MX_GOAL_SET, post_down_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 3, ADDR_MX_GOAL_SET, post_down_346, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 6, ADDR_MX_GOAL_SET, post_down_346, &dxl_err);
        // set post-up for 0,2,4
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 0, ADDR_MX_GOAL_SET, post_up_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 2, ADDR_MX_GOAL_SET, post_up_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 4, ADDR_MX_GOAL_SET, post_up_346, &dxl_err);

        // pause for 0.1s
        usleep(500000);

        // set pre-up for 1,3,6
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 1, ADDR_MX_GOAL_SET, pre_up_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 3, ADDR_MX_GOAL_SET, pre_up_346, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 6, ADDR_MX_GOAL_SET, pre_up_346, &dxl_err);
        // set pre-down for 0,2,4
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 0, ADDR_MX_GOAL_SET, pre_down_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 2, ADDR_MX_GOAL_SET, pre_down_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 4, ADDR_MX_GOAL_SET, pre_down_346, &dxl_err);

        // pause for 0.25s
        usleep(1000000);

        // set post-up for 1,3,6
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 1, ADDR_MX_GOAL_SET, post_up_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 3, ADDR_MX_GOAL_SET, post_up_346, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 6, ADDR_MX_GOAL_SET, post_up_346, &dxl_err);
        // set post-down for 0,2,4
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 0, ADDR_MX_GOAL_SET, post_down_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 2, ADDR_MX_GOAL_SET, post_down_012, &dxl_err);
        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, 4, ADDR_MX_GOAL_SET, post_down_346, &dxl_err);

        // pause for 0.1s
        usleep(500000);
        */

       for (int i = 0; i < 2; i++)
       {
            dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, dynamixel_ids[i], ADDR_MX_GOAL_SET, goal, &dxl_err);
       }

       goal += 100;
       goal = goal % 4096;
       usleep(1000000);

    }

    return 0;
}