/**
 * Network Interface interface file
 * 
 * Created by Vivaan Bahl, vrbahl
 * Last modified 4/15/2019
 */

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
}