#include "dynamixel_interface.h"

static dynamixel::PortHandler *port_handler;
static dynamixel::PacketHandler *packet_handler;

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
    for (int id : dynamixel_ids)
    {
        dxl_err = 0;
        dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ON, 1, &dxl_err);

        if (dxl_comm_res != COMM_SUCCESS || dxl_err)
        {
            std::cerr << "error: " << id << ":" << dxl_comm_res << ", " << packet_handler->getRxPacketError(dxl_err) << std::endl;
        }
        usleep(100000);
    }

    // set all the dynamixels to straight up
    uint16_t starting_positions[NUM_DYNAMIXELS] = {0};
    uint16_t starting_velocities[NUM_DYNAMIXELS] = {step_speed, step_speed, step_speed, step_speed, step_speed, step_speed};
    set_dynamixel_positions(dynamixel_ids, starting_positions, starting_velocities);
    return 0;
}

void set_dxl_velocity(uint8_t id, uint16_t desired_spd)
{
    uint8_t dxl_err;
    int16_t dxl_comm_res;

    if (is_inside_array(reversal_ids, id))
    {
        desired_spd += 1024;
    }

    dxl_comm_res = packet_handler->write2ByteTxRx(port_handler, id, ADDR_MX_VEL_SET, desired_spd, &dxl_err);
    check_dxl_result(id, dxl_err, dxl_comm_res);
}

uint16_t read_dynamixel_position(uint8_t id)
{
    uint8_t dxl_err;
    int16_t dxl_comm_res;
    uint16_t pos;
    dxl_comm_res = packet_handler->read2ByteTxRx(port_handler, id, ADDR_MX_GET_POS, &pos, &dxl_err);
    check_dxl_result(id, dxl_err, dxl_comm_res);

    return pos;
}

int8_t set_dynamixel_positions(const uint8_t id[NUM_DYNAMIXELS], uint16_t goal_position[NUM_DYNAMIXELS], uint16_t goal_velocities[NUM_DYNAMIXELS])
{

    // setup array for the completion
    uint16_t current_positions[NUM_DYNAMIXELS];
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        current_positions[i] = read_dynamixel_position(id[i]);
        set_dxl_velocity(id[i], goal_velocities[i]);
    }

    uint8_t num_dxls_left = 6;
    bool finished[] = {false, false, false, false, false, false};
    while (num_dxls_left)
    {
	std::cout << "Adjusted Positions: ";
        // iterate over each servo and see if it's gotten to its destination
        for (int i = 0; i < NUM_DYNAMIXELS; i++)
        {
            current_positions[i] = read_dynamixel_position(id[i]);
            uint16_t adjusted_pos = (current_positions[i] + dynamixel_offsets[i]) % DYN_ROTATION_TICKS;
            if (abs(adjusted_pos - goal_position[i]) < goal_tolerance && finished[i] == false)
            {
		    finished[i] = true;
                num_dxls_left--;
                set_dxl_velocity(id[i], 0);
		std::cout << "Dynamixel " << std::to_string(id[i]) << " finished" << std::endl;
            }

	    std::cout << std::to_string(adjusted_pos) << " ";
        }
	std::cout << std::endl;

        usleep(100000);
    }
    std::cout << "Finished Step" << std::endl;

    // return success or fail
    return 0;

}

int8_t dyn_intf_shutdown()
{
    for (int id : dynamixel_ids)
    {
        uint8_t dxl_err = 0;
        int16_t dxl_comm_res = packet_handler->write1ByteTxRx(port_handler, id, ADDR_MX_TORQUE_ON, 0, &dxl_err);
        check_dxl_result(id, dxl_err, dxl_comm_res);
    }
    port_handler->closePort();
    std::cout << "Closing port" << std::endl;
    return 0;
}
