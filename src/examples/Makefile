##################################################
# PROJECT: DXL Protocol 1.0 Read/Write Example Makefile
# AUTHOR : ROBOTIS Ltd.
##################################################

#---------------------------------------------------------------------
# Makefile template for projects using DXL SDK
#
# Please make sure to follow these instructions when setting up your
# own copy of this file:
#
#   1- Enter the name of the target (the TARGET variable)
#   2- Add additional source files to the SOURCES variable
#   3- Add additional static library objects to the OBJECTS variable
#      if necessary
#   4- Ensure that compiler flags, INCLUDES, and LIBRARIES are
#      appropriate to your needs
#
#
# This makefile will link against several libraries, not all of which
# are necessarily needed for your project.  Please feel free to
# remove libaries you do not need.
#---------------------------------------------------------------------

# *** ENTER THE TARGET NAME HERE ***
TARGET      = hellodynamixel 
TARGET2 = read_positions
TARGET3 = tripod_gait
TARGET4 = sync_write

# important directories used by assorted rules and other variables
DIR_DXL    = ../../DynamixelSDK/c++
DIR_OBJS   = .objects

# compiler options
CC          = gcc
CX          = g++
CCFLAGS     = -O2 -O3 -DLINUX -D_GNU_SOURCE -Wall $(INCLUDES) -g
CXFLAGS     = -O2 -O3 -DLINUX -D_GNU_SOURCE -Wall -std=c++11 $(INCLUDES) -g
LNKCC       = $(CX)
LNKFLAGS    = $(CXFLAGS) #-Wl,-rpath,$(DIR_THOR)/lib

#---------------------------------------------------------------------
# Core components (all of these are likely going to be needed)
#---------------------------------------------------------------------
INCLUDES   += -I$(DIR_DXL)/include/dynamixel_sdk
LIBRARIES  += -ldxl_sbc_cpp
LIBRARIES  += -lrt

#---------------------------------------------------------------------
# Files
#---------------------------------------------------------------------
SOURCES = hello_dynamixel.cpp
SOURCES2 = read_dyn_pos.cpp
SOURCES3 = tripod_gait.cpp
SOURCES4 = sync_write_example.cpp

OBJECTS  = $(addsuffix .o,$(addprefix $(DIR_OBJS)/,$(basename $(notdir $(SOURCES)))))
OBJECTS2  = $(addsuffix .o,$(addprefix $(DIR_OBJS)/,$(basename $(notdir $(SOURCES2)))))
OBJECTS3  = $(addsuffix .o,$(addprefix $(DIR_OBJS)/,$(basename $(notdir $(SOURCES3)))))
OBJECTS4  = $(addsuffix .o,$(addprefix $(DIR_OBJS)/,$(basename $(notdir $(SOURCES4)))))
#OBJETCS += *** ADDITIONAL STATIC LIBRARIES GO HERE ***


#---------------------------------------------------------------------
# Compiling Rules
#---------------------------------------------------------------------
$(TARGET): make_directory $(OBJECTS)
	$(LNKCC) $(LNKFLAGS) $(OBJECTS) -o $(TARGET) $(LIBRARIES)

$(TARGET2): make_directory $(OBJECTS2) 
	$(LNKCC) $(LNKFLAGS) $(OBJECTS2) -o $(TARGET2) $(LIBRARIES)

$(TARGET3): make_directory $(OBJECTS3) 
	$(LNKCC) $(LNKFLAGS) $(OBJECTS3) -o $(TARGET3) $(LIBRARIES)

$(TARGET4): make_directory $(OBJECTS4) 
	$(LNKCC) $(LNKFLAGS) $(OBJECTS4) -o $(TARGET4) $(LIBRARIES)

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(DIR_OBJS) core *~ *.a *.so *.lo

make_directory:
	mkdir -p $(DIR_OBJS)/

$(DIR_OBJS)/%.o: %.c
	$(CC) $(CCFLAGS) -c $? -o $@

$(DIR_OBJS)/%.o: %.cpp
	$(CX) $(CXFLAGS) -c $? -o $@

#---------------------------------------------------------------------
# End of Makefile
#---------------------------------------------------------------------
