/**
 * Interface file for the dynamixel interface
 * 
 * Created by Vivaan Bahl, vrbahl
 */

#ifndef DYN_INTF_H
#define DYN_INTF_H

#include <cstdint>

#include "dynamixel_sdk.h"
#include "config/config.h"
#include "instruction_parser/instruction_parser.h"

enum class DynInterfaceState : uint8_t
{
    INIT,
    IDLE,
    SEND_INSTR,
    READ_DATA,
    STOP_MOTORS,
    FINISH,
    SHUTDOWN,
    INVALID
};

enum class DynamixelErrorCodes : int8_t
{
    SUCCESS = 0,

    PORT_OPEN_FAILED = -1,
    BAUD_SET_FAILED = -2,
    DXL_ERR = -3,
    DXL_COMM_ERR = -4,
    VEL_CMD_ERR = -5,
    POS_READ_ERR = -6,
};

class DynamixelInterface
{
public:
    bool get_cmd_finished();
    int8_t set_instr(Instruction* instr);
    uint16_t *get_pos_data();
    bool *get_finished();
    int8_t set_shutdown(bool shutdown);
    int8_t set_run_command(bool run_command);
    bool is_port_open();

    DynamixelInterface();
    void tick();

private:
    DynInterfaceState current_state;

    dynamixel::PortHandler *port_handler;
    dynamixel::PacketHandler *adapter;
    dynamixel::GroupSyncWrite *group_sync_torque_toggle;
    dynamixel::GroupSyncWrite *group_sync_vel_set;
    dynamixel::GroupBulkRead *group_position_read;
    bool cmd_finished;
    Instruction *instr;
    uint16_t pos_data[NUM_DYNAMIXELS];
    bool finished[NUM_DYNAMIXELS];
    bool shutdown;
    bool run_command;
    bool port_closed;

    int8_t errcode;
    
    const std::string port_path = "/dev/ttyUSB0";
    const uint32_t baud_rate = 1000 * 1000;
    const uint8_t ADDR_MX_TORQUE_EN = 0x18;
    const uint8_t ADDR_MX_VEL_SET = 0x20;
    const uint8_t ADDR_MX_POS_GET = 0x24;
    const uint8_t TORQ_EN_PKT_LEN = 1;
    const uint8_t VEL_SET_PKT_LEN = 2;
    const uint8_t POS_GET_PKT_LEN = 2;
    const static uint8_t num_reversal = 3;
    const uint8_t reversal_ids[num_reversal] = {3, 4, 6};
    const uint16_t dynamixel_offsets[NUM_DYNAMIXELS] = { 150, 2277, 1069, 855, 211, 339 };
    const uint8_t dynamixel_ids[NUM_DYNAMIXELS] = {0, 1, 2, 3, 4, 6};
    const uint16_t DYN_ROTATION_TICKS = 4096;
    const uint16_t goal_tol = 50;

    DynamixelErrorCodes run_velocity_command();
    DynamixelErrorCodes read_pos_data();
    DynamixelErrorCodes compare_pos_data();
    DynamixelErrorCodes check_dxl_result(uint8_t dxl_err, int16_t dxl_comm_res);
};

#endif
