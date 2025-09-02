CXX ?= g++
CXXSTD ?= 17
CXXFLAGS ?= -O0 -g -std=c++$(CXXSTD) -DSC_ALLOW_DEPRECATED_IEEE_API -fsanitize=address
ifndef SYSTEMC_HOME
$(error Please set SYSTEMC_HOME to your SystemC installation path)
endif
UVM_SYSTEMC_HOME ?= $(abspath ../../../../third_party/uvm-systemc)
UVM_SC_INCDIRS := $(UVM_SYSTEMC_HOME)/include
UVM_SC_LIBDIRS := $(UVM_SYSTEMC_HOME)/lib $(UVM_SYSTEMC_HOME)/lib-macosx64
UVM_SC_INC := $(firstword $(foreach d,$(UVM_SC_INCDIRS),$(if $(wildcard $(d)),$(d),)))
UVM_SC_LIBDIR := $(firstword $(foreach d,$(UVM_SC_LIBDIRS),$(if $(wildcard $(d)),$(d),)))
LATEST_UVM_DYLIB := $(shell ls -1t $(addsuffix /libuvm-systemc*.dylib,$(UVM_SC_LIBDIRS)) 2>/dev/null | head -n1)
INC := -I$(SYSTEMC_HOME)/include -I$(UVM_SC_INC) -I../../tlm -I../env        -I../../../dv/sc/csr_utils -I../../../dv/sc/tl_agent        -I../../../dv/sc/dv_utils -I../../../dv/sc/bus_params_pkg -I../../../dv/sc/scoreboard
DEFAULT_FC4SC := ../../../../../third_party/fc4sc
ifneq ($(wildcard $(DEFAULT_FC4SC)/includes),)
  FC4SC_HOME ?= $(DEFAULT_FC4SC)
endif
ifdef FC4SC_HOME
  ifneq ($(wildcard $(FC4SC_HOME)/includes),)
    INC += -I$(FC4SC_HOME)/includes
  else ifneq ($(wildcard $(FC4SC_HOME)/include),)
    INC += -I$(FC4SC_HOME)/include
  endif
  CXXFLAGS += -DENABLE_FC4SC -DFC4SC_READY
endif
LIBS := -L$(SYSTEMC_HOME)/lib -lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib
# Link against locally built UVM-SystemC from detected lib dir
LIBS += -L$(UVM_SC_LIBDIR) -luvm-systemc -Wl,-rpath,$(UVM_SC_LIBDIR)
SRCS := main.cpp
TEST_SRCS := $(wildcard uart_*_test.cpp)
TESTS := $(patsubst %.cpp,%,$(TEST_SRCS))
all: $(TESTS)

# Link DUT implementation
DUT_SRC := ../../tlm/uart_reg_top.cpp

%: %.cpp main.cpp $(DUT_SRC)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIBS)
clean:
	rm -f $(TESTS)
PYTHON ?= $(shell [ -x $(abspath ../../../..)/.venv/bin/python ] && echo $(abspath ../../../..)/.venv/bin/python || command -v python3)

.PHONY: gen-collateral
gen-collateral:
	cd ../../../../util && $(PYTHON) gen_scdv_collateral.py --tmp-out uart
.PHONY: all clean
