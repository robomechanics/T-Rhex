#include <iostream>
#include <termios.h>
#include <unistd.h>

#include "dynamixel_sdk.h"

int main()
{
    dynamixel::PortHandler *port = dynamixel::PortHandler::getPortHandler("/dev/ttyUSB0");
    dynamixel::PacketHandler *pack = dynamixel::PacketHandler::getPacketHandler(1.0);

    if (!port->openPort())
    {
        std::cerr << "cant open port" << std::endl;
    }

    if (!port->setBaudRate(1000 * 1000))
    {
        std::cerr << "cant set baud" << std::endl;
    }

    uint8_t dxl_err;
    pack->write1ByteTxRx(port, 20, 0x18, 1, &dxl_err);


    for (int i = 0; i < 10; i++)
    {
        uint16_t pos = 2000;
        pack->write2ByteTxRx(port, 20, 30, pos, &dxl_err);
	usleep(500 * 1000);

        pos = 2700;
	pack->write2ByteTxRx(port, 20, 30, pos, &dxl_err);
	usleep(500 * 1000);
    }

    port->closePort();
    return 0;

}
