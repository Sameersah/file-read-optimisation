//
// Created by Sameer on 18/02/25.
//

#ifndef ICRASH_DATA_PROCESSOR_H
#define ICRASH_DATA_PROCESSOR_H

#include "CrashRecord.h"
#include <vector>
#include <string>

class ICrashDataProcessor {
public:
    virtual void loadData(const std::string& filename) = 0;
    virtual std::vector<CrashRecord> searchByDate(const std::string& date) const = 0;
    virtual std::vector<CrashRecord> searchByLocation(float lat, float lon, float radius) const = 0;
    virtual void displayStatistics() const = 0;
    virtual ~ICrashDataProcessor() {}
};

#endif // ICRASH_DATA_PROCESSOR_H
