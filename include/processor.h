#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <linux_parser.h>

class Processor {
    public:
        Processor();
        float Utilization(); 

    // Store current stats for short-term evaluation of CPU utilization.
    private:
        long last_jiffies_;
        long last_active_jiffies_;
};

#endif