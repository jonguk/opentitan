# Simple Makefile to build tb for SystemC

CXX ?= g++
CXXFLAGS ?= -O2 -g -std=c++17 -DSC_ALLOW_DEPRECATED_IEEE_API

ifndef SYSTEMC_HOME
$(error Please set SYSTEMC_HOME to your SystemC installation path)
endif

INC := -I$(SYSTEMC_HOME)/include -I../../tlm
LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc

SRCS := tb_aon_timer.cpp ../../tlm/aon_timer_reg_top.cpp

all: tb_aon_timer

tb_aon_timer: $(SRCS)
	$(CXX) $(CXXFLAGS) $(INC) $(SRCS) -o $$@ $(LIBS)

clean:
	rm -f tb_aon_timer

.PHONY: all clean
