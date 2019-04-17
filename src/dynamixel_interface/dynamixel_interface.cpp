#include "dynamixel_interface.h"

DynamixelInterface::DynamixelInterface()
{
    this->current_state = DynInterfaceState::INIT;
    this->port_handler = dynamixel::PortHandler::getPortHandler(this->port_path);
    this->adapter = dynamixel::PacketHandler::getPacketHandler(1.0);
}

void DynamixelInterface::tick()
{
    switch (this->current_state)
    {
        case DynInterfaceState::INIT:
        {
            break;
        }

        case DynInterfaceState::IDLE:
        {
            break;
        }

        case DynInterfaceState::SEND_INSTR:
        {
            break;
        }

        case DynInterfaceState::READ_DATA:
        {
            break;
        }

        case DynInterfaceState::STOP_MOTORS:
        {
            break;
        }

        case DynInterfaceState::FINISH:
        {
            break;
        }

        case DynInterfaceState::SHUTDOWN:
        {
            break;
        }
        
        default:
        {
            std::cerr << "Dynamixel Interface in invalid state! Errcode = " << errcode << std::endl;
            break;
        }
    }
}