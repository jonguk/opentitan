#pragma once
#include <uvm>

#define DV_CHECK(cond, msg) \
  do { if (!(cond)) uvm::uvm_report_error("DV_CHECK", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_EQ(a,b,msg) \
  do { if (!((a)==(b))) uvm::uvm_report_error("DV_CHECK_EQ", (msg), uvm::UVM_NONE); } while(0)
