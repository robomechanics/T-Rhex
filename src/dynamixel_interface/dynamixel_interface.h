/**
 * Interface file for the dynamixel interface
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

#ifndef DYN_INTF_H
#define DYN_INTF_H

#include <cstdint>

#include "dynamixel_sdk.h"
#include "config/config.h"
#include "instruction_parser/instruction_parser.h"

enum DynInterfaceState : uint8_t
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

class DynamixelInterface
{
public:
    bool get_cmd_finished();
    int8_t set_set_baud_rate(bool set_baud_rate);
    int8_t set_desired_baud(uint32_t desired_baud);
    int8_t set_instr(Instruction instr);
    uint16_t *get_pos_data();
    bool *get_finished();
    int8_t set_shutdown(bool shutdown);

    DynamixelInterface();
    void tick();

private:
    DynInterfaceState current_state;

    dynamixel::PortHandler *port_handler;
    dynamixel::PacketHandler *adapter;
    bool cmd_finished;
    bool set_baud_rate;
    uint32_t desired_baud;
    Instruction instr;
    uint16_t pos_data[NUM_DYNAMIXELS] pos_data;
    bool finished[NUM_DYNAMIXELS];
    bool shutdown;

    int8_t errcode;
    
    const std::string port_path = "/dev/ttyUSB0";
};

#endif