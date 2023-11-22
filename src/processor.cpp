#include "processor.h"
#include "linux_parser.h"

#include "assert.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float util;

    //long next_idle = LinuxParser::IdleJiffies();
    long next_active = LinuxParser::ActiveJiffies();
    long next_total = LinuxParser::Jiffies();

    if (active != next_active && total != next_active){
        long totald = next_total - total;
        long actived = next_active - active;

        
        util = (float) actived / (float) totald; // formulas are from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
                                                // from the provided references.
        active = next_active;
        total = next_total;

        assert(actived > 0);
        assert(totald > 0);
        
    } else{

        util = (float) active / (float) total;

        return util;

    }

    return util;
}