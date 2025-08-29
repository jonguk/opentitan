# Simple Makefile to build tb_uart against SystemC (and optionally UVM SystemC)

CXX ?= g++
CXXFLAGS ?= -O2 -g -std=c++17 -DSC_ALLOW_DEPRECATED_IEEE_API

# Set SYSTEMC_HOME to your SystemC installation root
ifndef SYSTEMC_HOME
$(error Please set SYSTEMC_HOME to your SystemC installation path)
endif

INC := -I$(SYSTEMC_HOME)/include -I../../tlm
LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc

# Optionally add UVM SystemC if available
ifdef UVM_SYSTEMC_HOME
INC += -I$(UVM_SYSTEMC_HOME)/include
LIBS += -L$(UVM_SYSTEMC_HOME)/lib -luvmsc
endif

SRCS := tb_uart.cpp ../../tlm/uart_reg_top.cpp

all: tb_uart

tb_uart: $(SRCS)
	$(CXX) $(CXXFLAGS) $(INC) $(SRCS) -o $@ $(LIBS)

clean:
	rm -f tb_uart

.PHONY: all clean


