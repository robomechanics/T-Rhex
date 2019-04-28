TARGET = trhex
DIR_DXL = ../DynamixelSDK/c++

DYN_INTF_SRC = dynamixel_interface/dynamixel_interface.cpp
INSTR_PARSER_SRC = instruction_parser/instruction_parser.cpp
MAIN_SRC = main/main.cpp
MICRO_SRC = microcontroller/microcontroller.cpp
NET_INTF_SRC = network_interface/network_interface.cpp

INCLUDES += -I$(DIR_DXL)/include/dynamixel_sdk
INCLUDES += -I./

LIBRARIES += -ldxl_x64_cpp
LIBRARIES += -lrt

SOURCES = $(DYN_INTF_SRC) $(INSTR_PARSER_SRC) $(MAIN_SRC) $(MICRO_SRC) $(NET_INTF_SRC)

all:
	g++ -O2 -DLINUX -D_GNU_SOURCE -Wall -Wextra -std=c++11 -m64 $(INCLUDES) $(SOURCES) -o $(TARGET) $(LIBRARIES)