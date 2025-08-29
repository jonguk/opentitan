CXX ?= g++
CXXFLAGS ?= -O2 -g -std=c++17 -DSC_ALLOW_DEPRECATED_IEEE_API
ifndef SYSTEMC_HOME
$(error Please set SYSTEMC_HOME to your SystemC installation path)
endif
UVM_SYSTEMC_HOME ?= /opt/homebrew/uvm-systemc
UVM_SC_INCDIRS := $(UVM_SYSTEMC_HOME)/include /opt/homebrew/include
UVM_SC_LIBDIRS := $(UVM_SYSTEMC_HOME)/lib $(UVM_SYSTEMC_HOME)/lib-macosarm64 /opt/homebrew/uvm-systemc/lib-macosarm64 /opt/homebrew/lib
UVM_SC_INC := $(firstword $(foreach d,$(UVM_SC_INCDIRS),$(if $(wildcard $(d)),$(d),)))
UVM_SC_LIBDIR := $(firstword $(foreach d,$(UVM_SC_LIBDIRS),$(if $(wildcard $(d)),$(d),)))
LATEST_UVM_DYLIB := $(shell ls -1t $(addsuffix /libuvm-systemc*.dylib,$(UVM_SC_LIBDIRS)) 2>/dev/null | head -n1)
INC := -I$(SYSTEMC_HOME)/include -I$(UVM_SC_INC) -I../../tlm -I../env -I../../../dv/sc/csr_utils -I../../../dv/sc/tl_agent -I../../../dv/sc/dv_utils -I../../../dv/sc/bus_params_pkg -I../../../dv/sc/scoreboard
# Prefer vendored fc4sc subtree if present
DEFAULT_FC4SC := ../../../third_party/fc4sc
ifneq ($(wildcard $(DEFAULT_FC4SC)/include),)
  FC4SC_HOME ?= $(DEFAULT_FC4SC)
endif
ifdef FC4SC_HOME
  CXXFLAGS += -DENABLE_FC4SC -DFC4SC_READY
  INC += -I$(FC4SC_HOME)/include
endif
LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc
ifeq ($(strip $(LATEST_UVM_DYLIB)),)
  LIBS += -L$(UVM_SC_LIBDIR) -luvm-systemc
else
  LIBS += $(LATEST_UVM_DYLIB)
endif
SRCS := main.cpp
DUT_SRC := ../../tlm/uart_reg_top.cpp
TEST_SRCS := $(wildcard uart_*_test.cpp)
TESTS := $(patsubst %.cpp,%,$(TEST_SRCS))
all: $(TESTS)
%: %.cpp main.cpp $(DUT_SRC)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIBS)
clean:
	rm -f $(TESTS)
.PHONY: all clean
