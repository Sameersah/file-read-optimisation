#ifndef MEMORY_USAGE_H
#define MEMORY_USAGE_H

#include <mach/mach.h>
#include <iostream>

class MemoryUsage {
public:
    static void printMemoryUsage(const std::string& methodName) {
        mach_task_basic_info info;
        mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
        if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
            std::cout << "🔹 Memory Used by " << methodName << " - Resident Size: "
                      << info.resident_size / (1024 * 1024) << " MB\n";
        } else {
            std::cerr << "❌ Failed to get memory usage info.\n";
        }
    }
};

#endif // MEMORY_USAGE_H
