

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
const static uint8_t ADDR_MX_VEL_SET = 0x20;
const static uint8_t ADDR_MX_GET_POS = 0x24;

static dynamixel::PortHandler *port_handler;
static dynamixel::PacketHandler *packet_handler;
const static int dynamixel_ids[] = { 0, 1, 2, 3, 4, 6 };
const static uint16_t pos_half_step = 1700;
const static uint16_t pos_ground_step = 3600;
const static uint16_t step_speed = 100;
static bool keep_walking = true;

void check_dxl_result(int id, uint8_t dxl_err, int16_t dxl_comm_res, std::string step, dynamixel::PacketHandler *ph)
{
    if (dxl_err || dxl_comm_res != COMM_SUCCESS)
    {
        std::cerr << "Error performing " << step << " on dxl " << id << ". err=" << ph->getRxPacketError(dxl_err) << " cr=" << ph->getTxRxResult(dxl_comm_res) << std::endl;
    }
}

void set_dxl_velocity(int id, uint16_t desired_spd, std::string step, dynamixel::PacketHandler *packet_handler, dynamixel::PortHandler *port_handler)
{
    uint8_t dxl_err;
    int16_t dxl_comm_res;
    dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, id, ADDR_MX_VEL_SET, desired_spd, &dxl_err);
    check_dxl_result(1, dxl_err, dxl_comm_res, step, packet_handler);
}

uint16_t read_dxl_position(int id, std::string step, dynamixel::PacketHandler *packet_handler, dynamixel::PortHandler *port_handler)
{
    uint8_t dxl_err;
    int16_t dxl_comm_res;
    uint16_t pos;
    dxl_comm_res = packet_handler->read2ByteTxRx(port_handler, id, ADDR_MX_GET_POS, &pos, &dxl_err);
    check_dxl_result(1, dxl_err, dxl_comm_res, step, packet_handler);

    return pos;
}
void signint_handler( int signum )
{
    keep_walking = false;
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

    uint8_t dxl_err = 0;
    int16_t dxl_comm_res = 0;

    for (int id : dynamixel_ids)
    {
        dxl_err = 0;
        dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 1, &dxl_err);

        if (dxl_comm_res != COMM_SUCCESS || dxl_err)
        {
            std::cerr << "error: " << id << ":" << dxl_comm_res << ", " << packet_handler->getRxPacketError(dxl_err) << std::endl;
        }
        usleep(100000);
    }

    // take the first step with the 613 tripod
    std::string step = "init 613 tripod";
    set_dxl_velocity(1, step_speed, step, packet_handler, port_handler);
    set_dxl_velocity(3, 1024+step_speed, step, packet_handler, port_handler);
    set_dxl_velocity(6, 1024+step_speed, step, packet_handler, port_handler);

    bool s1_done, s3_done, s6_done = false;
    uint16_t tol = 200;

    while (!(s1_done && s3_done && s6_done))
    {
        uint16_t s1_pos, s3_pos, s6_pos = 0;

        step = "wait for 613 tripod";
        s1_pos = read_dxl_position(1, step, packet_handler, port_handler);
        s3_pos = read_dxl_position(3, step, packet_handler, port_handler);
        s6_pos = read_dxl_position(6, step, packet_handler, port_handler);

        if (abs(s1_pos - pos_half_step) < tol)
        {
            set_dxl_velocity(1, 0, "stop s1", packet_handler, port_handler);
            s1_done = true;
        }
        if (abs(s3_pos - pos_half_step) < tol)
        {
            set_dxl_velocity(3, 0, "stop s3", packet_handler, port_handler);
            s3_done = true;
        }
        if (abs(s6_pos - pos_half_step) < tol)
        {
            set_dxl_velocity(6, 0, "stop s6", packet_handler, port_handler);
            s6_done = true;
        }
    }

    bool tripod_613_up = true;

    while (keep_walking)
    {
        // set the moving speed of both tripods
        step = "start moving tripods";
        set_dxl_velocity(0, step_speed, step, packet_handler, port_handler);
        set_dxl_velocity(1, step_speed, step, packet_handler, port_handler);
        set_dxl_velocity(2, step_speed, step, packet_handler, port_handler);
        set_dxl_velocity(3, 1024+step_speed, step, packet_handler, port_handler);
        set_dxl_velocity(4, 1024+step_speed, step, packet_handler, port_handler);
        set_dxl_velocity(6, 1024+step_speed, step, packet_handler, port_handler);

        // wait on their completions
        bool servos_completed[] = {false, false, false, false, false, false};
        uint16_t completion_positions[6] = {0,0,0,0,0,0};
        if (tripod_613_up)
        {
            completion_positions[0] = pos_half_step;
            completion_positions[1] = pos_ground_step;
            completion_positions[2] = pos_half_step;
            completion_positions[3] = pos_ground_step;
            completion_positions[4] = pos_half_step;
            completion_positions[5] = pos_ground_step;
        }
        else
        {
            completion_positions[0] = pos_ground_step;
            completion_positions[1] = pos_half_step;
            completion_positions[2] = pos_ground_step;
            completion_positions[3] = pos_half_step;
            completion_positions[4] = pos_ground_step;
            completion_positions[5] = pos_half_step;
        }

        step = "wait for tripod completion";
        while(!(servos_completed[0] && servos_completed[1] && servos_completed[2] && servos_completed[3] && servos_completed[4] && servos_completed[5]))
        {
            for (int i = 0; i < 6; i++)
            {
                uint16_t pos = read_dxl_position(dynamixel_ids[i], step, packet_handler, port_handler);
                if (abs(pos - completion_positions[i]) < tol)
                {
                    set_dxl_velocity(dynamixel_ids[i], 0, "stop servo " + i, packet_handler, port_handler);
                    servos_completed[i] = true;
                }
            }
        }
        std::cout << "finished step" << std::endl;

        // wait for 0.5s before taking another step
        usleep(500000);

        tripod_613_up = !tripod_613_up;
    }

    // shutdown
    for (int id : dynamixel_ids)
    {
        set_dxl_velocity(id, 0, "stop servo", packet_handler, port_handler);
        uint8_t dxl_err = 0;
        int16_t dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ENABLE, 0, &dxl_err);
        check_dxl_result(id, dxl_err, dxl_comm_res, "Shutdown", packet_handler);
    }
    port_handler->closePort();
    std::cout << "Closing port" << std::endl;
    return 0;
}