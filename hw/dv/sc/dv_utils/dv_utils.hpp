#pragma once
#include <uvm>
#include <string>

#define DV_CHECK(cond, msg) \
  do { if (!(cond)) uvm::uvm_report_error("DV_CHECK", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_EQ(a,b,msg) \
  do { if (!((a)==(b))) uvm::uvm_report_error("DV_CHECK_EQ", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_NE(a,b,msg) \
  do { if (!((a)!=(b))) uvm::uvm_report_error("DV_CHECK_NE", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_FATAL(cond, msg) \
  do { if (!(cond)) uvm::uvm_report_fatal("DV_CHECK_FATAL", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_EQ_FATAL(a,b,msg) \
  do { if (!((a)==(b))) uvm::uvm_report_fatal("DV_CHECK_EQ_FATAL", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_GT_FATAL(a,b,msg) \
  do { if (!((a)>(b))) uvm::uvm_report_fatal("DV_CHECK_GT_FATAL", (msg), uvm::UVM_NONE); } while(0)

#define DV_CHECK_STRNE_FATAL(s, lit, msg) \
  do { if (std::string(s).empty()) uvm::uvm_report_fatal("DV_CHECK_STRNE_FATAL", (msg), uvm::UVM_NONE); } while(0)
