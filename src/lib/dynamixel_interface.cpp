#include "dynamixel_interface.h"

static dynamixel::PortHandler *port_handler;
static dynamixel::PacketHandler *packet_handler;
static dynamixel::GroupSyncWrite group_sync_write_te;
static dynamixel::GroupSyncWrite group_pos_sync;
static dynamixel::GroupBulkRead group_bulk_read;

void check_dxl_result(int id, uint8_t dxl_err, int16_t dxl_comm_res)
{
    if (dxl_err || dxl_comm_res != COMM_SUCCESS)
    {
        std::cerr << "Error! dxl_err=" << packet_handler->getRxPacketError(dxl_err) << " cr=" << packet_handler->getTxRxResult(dxl_comm_res) << std::endl;
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

int8_t dyn_intf_init()
{
    port_handler = dynamixel::PortHandler::getPortHandler(device_port_path.c_str());
    packet_handler = dynamixel::PacketHandler::getPacketHandler(protocol_version);
    group_sync_write_te(port_handler, packet_handler, ADDR_MX_TORQUE_ENABLE, 1);
    goal_pos_sync(port_handler, packet_handler, ADDR_MX_GOAL_TORQUE, 2);
    group_bulk_read(port_handler, packet_handler);

    for (int id : dynamixel_ids)
    {
        bool success = group_bulk_read.addParam(id, ADDR_MX_GET_POS, 2);
        if (!success)
        {
            std::cerr << "Unable to add to reader" << std::endl;
        }
    }

    if (port_handler->openPort())
    {
        std::cout << "Opened Dynamixel Port" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open Dynamixel Port" << std::endl;
        return 1;
    }

    if (port_handler->setBaudRate(baudrate))
    {
        std::cout << "Set baud rate to " << std::to_string(baudrate) << std::endl;
    }
    else
    {
        std::cerr << "Unable to set baud rate" << std::endl;
        return 2;
    }
    
    uint8_t dxl_err;
    int16_t dxl_comm_res;
    uint8_t torque_enable = 1;
    for (int id : dynamixel_ids)
    {
        bool add_param_success = group_sync_write_te.addParam(id, &torque_enable);
        if (!add_param_success)
        {
            std::cerr << "failed to add param" << std::endl;
        }
    }

    check_dxl_result(0, 0, group_sync_write_te.txPacket());
    group_sync_write_te.clearParam();

    // set all the dynamixels to straight up
    uint16_t starting_positions[NUM_DYNAMIXELS] = {0};
    uint16_t starting_velocities[NUM_DYNAMIXELS] = {step_speed, step_speed, step_speed, step_speed, step_speed, step_speed};
    set_dynamixel_positions(dynamixel_ids, starting_positions, starting_velocities);
    return 0;
}

int8_t set_dynamixel_positions(const uint8_t id[NUM_DYNAMIXELS], uint16_t goal_position[NUM_DYNAMIXELS], uint16_t goal_velocities[NUM_DYNAMIXELS])
{

    // command them to the velocities
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        uint16_t dyn_vel = 0;
        if (is_inside_array(reversal_ids, dynamixel_ids[i]))
        {
            dyn_vel = goal_velocities[i] + 1024;
        }
        else
        {
            dyn_vel = goal_velocities[i];
        }
        uint8_t vel_data[2] = { DXL_LOBYTE(dyn_vel), DXL_HIBYTE(dyn_vel) };

        bool success = goal_pos_sync.addParam(dynamixel_ids[i], vel_data);

        if (!success)
        {
            std::cerr << "couldn't add vel param" << std::endl;
        }
    }

    check_dxl_result(0, 0, group_pos_sync.txPacket());
    group_pos_sync.clearParam();

    uint16_t current_positions[NUM_DYNAMIXELS];

    bool all_dyns_finished = false;
    bool finished[] = {false, false, false, false, false, false};
    while (!all_dyns_finished)
    {
        check_dxl_result(0, 0, group_bulk_read.txRxPacket());

        for (int i = 0; i < NUM_DYNAMIXELS; i++)
        {
            uint8_t dxl_err;
            int16_t dxl_comm_res = 0;
            uint16_t curr_pos = group_bulk_read.getData(dynamixel_ids[i], ADDR_MX_GET_POS, 2);
            uint16_t adjusted_pos = (current_pos + dynamixel_offsets[i]) % DYN_ROTATION_TICKS;
            if (abs(adjusted_pos - goal_position[i]) < goal_tolerance && !finished[i])
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

        usleep(10 * 1000);
    
    }

    std::cout << "finished step" << std::endl;

    // return success or fail
    return 0;
}

int8_t dyn_intf_shutdown()
{
    for (int id : dynamixel_ids)
    {
        uint8_t disable = 0;
        bool add_param_success = group_sync_write_te.addParam(id, &disable);
        if (!add_param_success)
        {
            std::cerr << "failed to add param" << std::endl;
        }
    }
    port_handler->closePort();
    std::cout << "Closing port" << std::endl;
    return 0;
}
