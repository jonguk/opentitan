#include <systemc>
int sc_main(int argc, char** argv) {
  sc_core::sc_report_handler::set_actions(SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_, sc_core::SC_DO_NOTHING);
  sc_core::sc_start(sc_core::SC_ZERO_TIME);
  return 0;
}
