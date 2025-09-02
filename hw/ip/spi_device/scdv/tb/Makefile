# Simple Makefile to build tb for SystemC

CXX ?= g++
CXXFLAGS ?= -O2 -g -std=c++17 -DSC_ALLOW_DEPRECATED_IEEE_API

ifndef SYSTEMC_HOME
$(error Please set SYSTEMC_HOME to your SystemC installation path)
endif

INC := -I$(SYSTEMC_HOME)/include -I../../tlm
LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc

SRCS := tb_spi_device.cpp ../../tlm/spi_device_reg_top.cpp

all: tb_spi_device

tb_spi_device: $(SRCS)
	$(CXX) $(CXXFLAGS) $(INC) $(SRCS) -o $$@ $(LIBS)

clean:
	rm -f tb_spi_device

.PHONY: all clean
