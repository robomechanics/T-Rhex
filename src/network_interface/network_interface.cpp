#include "network_interface.h"

NetworkInterface::NetworkInterface(bool run_server)
{
    this->current_state = NetInterfaceState::INIT;
    if (run_server)
    {
        // open server socket
        NetErrorCodes errcode = server.open_socket();
    }
}

NetErrorCodes NetworkSocket::is_connection_active()
{
    if (connection_active)
    {
        return NetErrorCodes::CONNECTION_ACTIVE;
    }

    return NetErrorCodes::CONNECTION_INACTIVE;
}

NetErrorCodes NetworkSocket::open_socket()
{
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return NetErrorCodes::SOCKET_CREATE_FAILED;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "Failed to set opt" << std::endl;
        return NetErrorCodes::OPT_SET_FAILED;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(port_num); 

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        std::cerr << "Failed to bind socket" << std::endl;
        return NetErrorCodes::BIND_FAILED;
    } 

    if (listen(server_fd, 3) < 0) 
    { 
        std::cerr << "Failed to listen" << std::endl;
        return NetErrorCodes::LISTEN_FAILED;
    }    

    if ((new_socket == accept(server_fd, (struct sockaddr * )&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("failed to accept a client connection");
        connection_active = false;
        return;
    }

    connection_active = true;

    return NetErrorCodes::SUCCESS;
}

void NetworkInterface::tick()
{
    switch(current_state)
    {
        case NetInterfaceState::INIT:
        {
            this->current_state = NetInterfaceState::WAIT_FOR_CONN;

            break;
        }

        case NetInterfaceState::WAIT_FOR_CONN:
        {
            // ping server to see if we have a connection
            NetErrorCodes active_connection = server.is_connection_active();

            if (active_connection == NetErrorCodes::CONNECTION_ACTIVE)
            {
                this->current_state = NetInterfaceState::IDLE;
            }
            else if (active_connection == NetErrorCodes::CONNECTION_INACTIVE)
            {
                this->current_state = NetInterfaceState::WAIT_FOR_CONN;
            }
            else
            {
                this->current_state = NetInterfaceState::INVALID;
            }

            break;
        }

        case NetInterfaceState::GET_NEW_LEG:
        {
            // get leg data and write it to server
            write_leg_data();

            this->current_state = NetInterfaceState::GET_POWER_DATA;
            break;
        }

        case NetInterfaceState::GET_POWER_DATA:
        {
            // get battery voltage and current and write it to server

            // sike we're publishing the current instruction instead
            write_inst_data();
            this->current_state = NetInterfaceState::IDLE;
            break;
        }

        case NetInterfaceState::IDLE:
        {
            // no action
            this->current_state = NetInterfaceState::GET_NEW_LEG;
            break;
        }

        default:
        {
            break;
        }
    }
}

NetErrorCodes NetworkSocket::write(uint8_t bytes[], uint16_t bytes_len)
{
    send(new_socket, bytes, bytes_len, 0);
}

NetErrorCodes NetworkSocket::read_socket(uint8_t *bytes, uint16_t max_bytes_read)
{
    read(new_socket, bytes, max_bytes_read);
}

void NetworkInterface::write_leg_data()
{
    uint16_t num_bytes = NUM_LEGS * 2 + 2; // 2 bytes per leg, 1 header, 1 footer
    uint8_t leg_data_bytes[num_bytes];
    leg_data_bytes[0] = HDR_LEG;
    uint8_t index = 1;
    for (int i = 0; i < NUM_LEGS; i++)
    {
        leg_data_bytes[index] = (leg_data[i] >> 8) & 0xFF;
        index += 1;
        leg_data_bytes[index] = (leg_data[i]) & 0xFF;
        index += 1;
    }
    leg_data_bytes[num_bytes-1] = FTR_BYTE;

    server.write(leg_data_bytes, num_bytes);
}

void NetworkInterface::write_inst_data()
{
    uint16_t num_bytes = NUM_DYNAMIXELS * 2 + 2; // 2 bytes per leg, 1 header, 1 footer
    uint8_t leg_data_bytes[num_bytes];
    leg_data_bytes[0] = HDR_INS;
    uint8_t index = 1;
    for (int i = 0; i < NUM_DYNAMIXELS; i++)
    {
        leg_data_bytes[index] = (current_instruction->goal_positions[i] >> 8) & 0xFF;
        index += 1;
        leg_data_bytes[index] = (current_instruction->goal_positions[i]) & 0xFF;
        index += 1;
    }
    leg_data_bytes[num_bytes-1] = FTR_BYTE;

    server.write(leg_data_bytes, num_bytes);
}

int8_t NetworkInterface::set_leg_data(uint16_t leg_data[NUM_LEGS])
{
    this->leg_data = leg_data;
    return 0;
}

void NetworkInterface::set_current_instruction(Instruction *current_instruction)
{
    this->current_instruction = current_instruction;
}