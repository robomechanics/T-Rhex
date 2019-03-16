#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "dynamixel_sdk.h"

const static std::string device_port_path = "/dev/ttyUSB0";
const static uint32_t baudrate = 1000000;
const static float protocol_version = 1.0;
const static uint8_t ADDR_MX_TORQUE_ENABLE = 0x18;
const static uint8_t ADDR_MX_LED_CTL = 0x19;
const static uint8_t ADDR_MX_GOAL_SET = 0x1E;

const static int dynamixel_ids[] = { 0, 1, 2, 3, 4, 6 };
const static uint16_t goal_position = 1000;

void print_dynamixel_status(int dxl_comm_result, int dxl_err, int id, std::string cmd)
{
    if (dxl_comm_result == COMM_SUCCESS && dxl_err == 0)
    {
        std::cout << "set Dynamixel " << id << " " << cmd << std::endl;
    }
    else
    {
        std::cerr << "Error setting Dynamixel " << id << " " << cmd << ": " << dxl_comm_result << ", " << dxl_err << std::endl;
    }
}

int main()
{

    dynamixel::PortHandler *port_handler = dynamixel::PortHandler::getPortHandler(device_port_path.c_str());
    dynamixel::PacketHandler *packet_handler = dynamixel::PacketHandler::getPacketHandler(protocol_version);


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
    /*

    for (int id : dynamixel_ids)
    {

        uint8_t dxl_err = 0;
        int16_t dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 1, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, id, "enable torque");

        dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_LED_CTL, 1, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, id, "turn led on");

        // sleep for 1s
        usleep(1000000);

        dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_LED_CTL, 0, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, id, "turn led off");

        //sleep for 0.5s
        usleep(500000);

        dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, id, ADDR_MX_GOAL_SET, goal_position, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, id, "set goal pos");
        
        // TEMP sleep for 5s to settle, replace with reading the present position
        usleep(500000);

        // disable torque
        dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 0, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, id, "disable torque");
    }
    */

    uint16_t ADDR_GOAL_TORQUE = 0x20;
    uint8_t dxl_err = 0;
    uint8_t id = 0;
    int16_t dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 1, &dxl_err);
    print_dynamixel_status(dxl_comm_res, dxl_err, id, "enable torque");

    while (true)
    {
        for (int id : dynamixel_ids) {
        packet_handler->write2ByteTxRx(port_handler, id, ADDR_GOAL_TORQUE, 1024+100, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, 0, "set goal torque");
        }

        usleep(500000);

        for (int id : dynamixel_ids) {
        packet_handler->write2ByteTxRx(port_handler, id, ADDR_GOAL_TORQUE, 0, &dxl_err);
        print_dynamixel_status(dxl_comm_res, dxl_err, 0, "stop goal torque");
        }

        usleep(500000);

    }

    port_handler->closePort();



    return 0;

}
