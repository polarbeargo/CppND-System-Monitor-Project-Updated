#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() {
  double utilization(0);
  long idle_ticks = LinuxParser::IdleJiffies();
  long active_ticks = LinuxParser::ActiveJiffies();
  long duration_activate(active_ticks - cached_active_ticks_);
  long duration_idle(idle_ticks - cached_idle_ticks_);
  long duration {duration_activate + duration_idle};
  utilization = static_cast<double>(duration_activate) / duration;
  cached_active_ticks_ = active_ticks;
  cached_idle_ticks_ = idle_ticks;
  return utilization;
}