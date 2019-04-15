/**
 * Network Interface interface file
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

#ifndef NETW_INTF_H
#define NETW_INTF_H

#include <cstdint>

#include "microcontroller/microcontroller.h"
#include "config/config.h"

enum NetInterfaceState
{
    INIT,
    WAIT_FOR_CONN,
    GET_NEW_LEG,
    GET_POWER_DATA,
    IDLE,
    INVALID
};

class NetworkSocket
{

};

class NetworkInterface
{
public:
    int8_t set_leg_data(uint16_t leg_data[NUM_DYNAMIXELS]);

private:
    NetInterfaceState current_state;

    NetworkSocket server;
    Microcontroller micro;
    uint16_t leg_data[NUM_DYNAMIXELS];
    uint32_t batt_voltage;
    uint32_t batt_current;
};

#endif