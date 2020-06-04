/**
 * Network Interface interface file
 * 
 * Created by Vivaan Bahl, vrbahl
 */

#ifndef NETW_INTF_H
#define NETW_INTF_H

#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "microcontroller/microcontroller.h"
#include "config/config.h"
#include "instruction_parser/instruction_parser.h"

enum class NetInterfaceState : uint8_t
{
    INIT,
    WAIT_FOR_CONN,
    GET_NEW_LEG,
    GET_POWER_DATA,
    IDLE,
    INVALID
};

enum class NetErrorCodes : int8_t
{
    SUCCESS = 0,

    CONNECTION_ACTIVE = 1,
    CONNECTION_INACTIVE = 2,

    GENERIC_ERR = -1,
    SOCKET_CREATE_FAILED = -2,
    OPT_SET_FAILED = -3,
    BIND_FAILED = -4,
    LISTEN_FAILED = -5,
    ACCEPT_FAILED = -6,
};

class NetworkSocket
{
public:
    const int port_num = 42069;

    NetErrorCodes open_socket();
    NetErrorCodes is_connection_active();
    NetErrorCodes write(uint8_t bytes[], uint16_t bytes_len);
    NetErrorCodes read_socket(uint8_t *bytes, uint16_t max_bytes_read);
private:
    bool connection_active = false;
    int server_fd;
    int new_socket;
    int valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    std::thread client_accept_thread;
};

class NetworkInterface
{
public:
    int8_t set_leg_data(uint16_t leg_data[NUM_LEGS]);
    void set_current_instruction(Instruction *curr_ins);

    NetworkInterface(bool run_server);
    void tick();

private:
    NetInterfaceState current_state;

    NetworkSocket server;
    uint16_t leg_data[NUM_LEGS];
    uint32_t batt_voltage;
    uint32_t batt_current;
    Instruction *current_instruction;

    void write_leg_data();
    void write_inst_data();

    const static uint8_t HDR_LEG = 0x69;
    const static uint8_t HDR_INS = 0x42;
    const static uint8_t FTR_BYTE = 0x0A;
};

#endif