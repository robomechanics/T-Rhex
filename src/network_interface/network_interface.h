/**
 * Network Interface interface file
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

#ifndef NETW_INTF_H
#define NETW_INTF_H

enum NetInterfaceState
{
    INIT,
    WAIT_FOR_CONN,
    GET_NEW_LEG,
    GET_POWER_DATA,
    IDLE,
    INVALID
};

class NetworkInterface
{
private:
    NetInterfaceState current_state;
};

#endif