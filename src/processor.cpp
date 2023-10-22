#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // Return the aggregate CPU utilization
  long total_old, total_new, active_new, idle_old, idle_new;
  total_new = LinuxParser::Jiffies();
  active_new = LinuxParser::ActiveJiffies();
  idle_new = LinuxParser::IdleJiffies();

  total_old = m_total;
  idle_old = m_idle;

  UpdateStats(idle_new, active_new, total_new);

  float totalDelta = float(total_new) - float(total_old);
  float idleDetla = float(idle_new) - float(idle_old);

  float utilization = (totalDelta - idleDetla) / totalDelta;
  return utilization;
}

void Processor::UpdateStats(long idle, long active, long total) {
  m_idle = idle;
  m_active = active;
  m_total = total;
}