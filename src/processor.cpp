#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float utilization(0);
  long idle_ticks = LinuxParser::IdleJiffies();
  long active_tick = LinuxParser::ActiveJiffies();
  long duration_activate(active_tick - cached_active_ticks_);
  long duration_idle(idle_ticks - cached_idle_ticks_);
  long duration {duration_activate + duration_idle};
  utilization = static_cast<float>(duration_activate) / duration;
  cached_active_tick = active_tick;
  cached_idle_tick = idle_ticks;
  return utilization;
}