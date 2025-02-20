//
// Created by Sameer on 18/02/25.
//

#ifndef ICRASH_DATA_PROCESSOR_H
#define ICRASH_DATA_PROCESSOR_H

#include "SequentialProcessor/common/CrashRecord.h"
#include <vector>
#include <string>

class ICrashDataProcessor {
public:
    virtual ~ICrashDataProcessor() = default;

    //read data
    virtual void loadData(const std::string& filename) = 0;

    // APIs for range based searching
    [[nodiscard]] virtual int getCrashesInDateRange(const std::string& start_date, const std::string& end_date) = 0;
    [[nodiscard]] virtual int getCrashesByInjuryCountRange(int min_injuries, int max_injuries) = 0;
    [[nodiscard]] virtual int getCrashesByLocationRange(float lat, float lon, float radius) = 0;

    [[nodiscard]] virtual std::chrono::duration<double> getDataLoadDuration() const = 0;
    [[nodiscard]] virtual std::chrono::duration<double> getDateRangeSearchingDuration() const = 0;
    [[nodiscard]] virtual std::chrono::duration<double> getInjuryRangeSearchingDuration() const = 0;
    [[nodiscard]] virtual std::chrono::duration<double> getLocationRangeSearchingDuration() const = 0;

};

#endif // ICRASH_DATA_PROCESSOR_H
