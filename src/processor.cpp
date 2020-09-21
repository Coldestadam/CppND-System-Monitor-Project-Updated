#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long int active_jiffies = LinuxParser::ActiveJiffies();
    long int idle_jiffies = LinuxParser::IdleJiffies();
    long int total = active_jiffies + idle_jiffies;

    return ((total - idle_jiffies)*1.0) / total;
}