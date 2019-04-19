#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <iostream>

#include "dynamixel_sdk.h"

const static std::string device_port_path = "/dev/ttyUSB0";
const static uint32_t baudrate = 1000 * 1000;
const static float protocol_version = 1.0;
const static uint8_t ADDR_MX_TORQUE_ENABLE = 0x18;
const static uint8_t ADDR_MX_GOAL_TORQUE = 0x20;
const static uint8_t ADDR_MX_GET_POS = 0x24;
static bool send_vel = true;

const static uint8_t dynamixel_ids[] = { 0, 1, 2, 3, 4, 6 };
const static uint8_t reversal_ids[] = {3, 4, 6};
const static uint8_t num_reversal = 3;
const static uint8_t NUM_DYNAMIXELS = 6;
static uint16_t goal_pos = 0;
const static uint16_t goal_step = 1000;
const static uint16_t DYN_ROTATION_TICKS = 4096;
const static uint16_t vel = 100;

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

bool is_inside_array(const uint8_t arr[num_reversal], uint8_t e)
{
    for (int i = 0; i < num_reversal; i++)
    {
        uint8_t c = arr[i];
        if (e == c)
        {
            return true;
        }
    }
    return false;
}

void signint_handler( int signum )
{
    send_vel = false;
}


int main()
{
    dynamixel::PortHandler *port_handler = dynamixel::PortHandler::getPortHandler(device_port_path.c_str());
    dynamixel::PacketHandler *packet_handler = dynamixel::PacketHandler::getPacketHandler(protocol_version);
    dynamixel::GroupSyncWrite group_sync_write_te(port_handler, packet_handler, ADDR_MX_TORQUE_ENABLE, 1);
    dynamixel::GroupSyncWrite goal_pos_sync(port_handler, packet_handler, ADDR_MX_GOAL_TORQUE, 2);
    dynamixel::GroupBulkRead group_bulk_read(port_handler, packet_handler);

    // setup reader
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        bool success = group_bulk_read.addParam(dynamixel_ids[i], ADDR_MX_GET_POS, 2);
        if (!success)
        {
            std::cerr << "unable to add to reader" << std::endl;
        }
    }

    if (port_handler->openPort())
    {
        std::cout << "opened port" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open port" << std::endl;
    }

    if (port_handler->setBaudRate(baudrate))
    {
        std::cout << "set baud rate" << std::endl;
    }
    else
    {
        std::cerr << "failed to set baud rate" << std::endl;
    }

    uint8_t torque_en_data[1] = {1};
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        bool add_param_success = group_sync_write_te.addParam(dynamixel_ids[i], torque_en_data);
        if (!add_param_success)
        {
            std::cerr << "failed to add param" << std::endl;
        }
    }

    print_dynamixel_status(group_sync_write_te.tx_packet(), 0, 7, "torque enable");
    group_sync_write_te.clearParam();

    while (send_vel)
    {

        for (int i = 0; i < NUM_DYNAMIXELS; i++)
        {
            uint16_t dyn_vel = rand() % 100;
            if (is_inside_array(reversal_ids, dynamixel_ids[i]))
            {
                dyn_vel += vel + 1024;
            }
            else
            {
                dyn_vel += vel;
            }
            uint8_t vel_data[2] = { DXL_LOBYTE(dyn_vel), DXL_HIBYTE(dyn_vel) };

            bool success = goal_pos_sync.addParam(dynamixel_ids[i], vel_data);

            if (!success)
            {
                std::cerr << "couldn't add goal param" << std::endl;
            }
        }

        print_dynamixel_status(goal_pos_sync.txPacket(), 0, 7, "set vel");
        goal_pos_sync.clearParam();


        bool all_dyns_finished = false;
        bool finished[] = {false, false, false, false, false, false};
        while (!all_dyns_finished)
        {
            print_dynamixel_status(group_bulk_read.txRxPacket(), 0, 7, "reading positions");

            for (int i = 0; i < NUM_DYNAMIXELS; i++)
            {
                uint8_t dxl_err;
                int16_t dxl_comm_res;
                uint16_t curr_pos = group_bullk_read.getData(dynamixel_ids[i], ADDR_MX_GET_POS, 2);
                if (abs(curr_pos - goal_pos) < 100 && !finished[i])
                {
                    finished[i] = true;
                    packet_handler->write2ByteTxRx(port_handler, dynamixel_ids[i], ADDR_MX_GOAL_TORQUE, 0, &dxl_err);
                    print_dynamixel_status(dxl_comm_res, dxl_err, dynamixel_ids[i], "stop goal torque");

                    all_dyns_finished = true;
                    for (int i = 0; i < NUM_DYNAMIXELS; i++)
                    {
                        if (!finished[i])
                        {
                            all_dyns_finished = false;
                        }
                    }
                }
            }

            usleep(100 * 1000);
        
        }

        goal_pos += goal_step;
        goal_pos = goal_pos % DYN_ROTATION_TICKS;

        usleep(5 * 1000 * 1000);

    }

    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        uint8_t disable = 0;
        bool add_param_success = group_sync_write_te.addParam(dynamixel_ids[i], &disable);
        if (!add_param_success)
        {
            std::cerr << "failed to add param" << std::endl;
        }
    }

    print_dynamixel_status(group_sync_write_te.tx_packet(), 0, 7, "torque disable");
    group_sync_write_te.clearParam();

    port_handler->closePort();

    return 0;
}
