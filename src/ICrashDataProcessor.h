//
// Created by Sameer on 18/02/25.
//

#ifndef ICRASH_DATA_PROCESSOR_H
#define ICRASH_DATA_PROCESSOR_H

#include "SequentialProcessor/CrashRecord.h"
#include <vector>
#include <string>

class ICrashDataProcessor {
public:
    virtual ~ICrashDataProcessor() = default;

    //read data
    virtual void loadData(const std::string& filename) = 0;

    // APIs for range based searching
    virtual std::vector<CrashRecord> getCrashesInDateRange(const std::string& start_date, const std::string& end_date) const = 0;
    virtual std::vector<CrashRecord> getCrashesByInjuryCountRange(int min_injuries, int max_injuries) const = 0;
    virtual std::vector<CrashRecord> getCrashesByLocationRange(float lat, float lon, float radius) const  =0;

};

#endif // ICRASH_DATA_PROCESSOR_H
