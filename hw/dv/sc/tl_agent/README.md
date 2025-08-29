# TL-UL Agent (SystemC/UVM-SC)

Skeleton agent mirroring `hw/dv/sv/tl_agent` structure:
- tl_item.hpp: transaction
- tl_sequencer.hpp: sequencer
- tl_driver.hpp: driver (initiator)
- tl_monitor.hpp: monitor (snoop bus events)
- tl_agent.hpp: composite agent

This is a minimal scaffold to unblock tests; functionality can be extended.
