//
// Created by Sameer on 18/02/25.
//

#ifndef CRASH_DATA_PROCESSOR_H
#define CRASH_DATA_PROCESSOR_H

#include "../ICrashDataProcessor.h"
#include <unordered_map>
#include <iostream>


class CrashDataProcessor : public ICrashDataProcessor {
private:
    std::vector<CrashRecord> records;
    std::unordered_map<std::string, std::vector<CrashRecord>> dateIndex;
    std::chrono::duration<double> data_load_duration = {};
    std::chrono::duration<double> date_range_Searching_duration = {};
    std::chrono::duration<double> injury_range_Searching_duration = {};
    std::chrono::duration<double> location_range_Searching_duration = {};

public:
    //read data
     void loadData(const std::string& filename) override;


    // APIs for range based searching
     [[nodiscard]] std::vector<CrashRecord> getCrashesInDateRange(const std::string& start_date, const std::string& end_date) const override;
     [[nodiscard]] std::vector<CrashRecord> getCrashesByInjuryCountRange(int min_injuries, int max_injuries) const override;
     [[nodiscard]] std::vector<CrashRecord> getCrashesByLocationRange(float lat, float lon, float radius)const override;

    //getters
    [[nodiscard]] std::chrono::duration<double> getDataLoadDuration() const { return data_load_duration; }
    [[nodiscard]] std::chrono::duration<double> getDateRangeSearchingDuration() const { return date_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getInjuryRangeSearchingDuration() const { return injury_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getLocationRangeSearchingDuration() const { return location_range_Searching_duration; }

};

#endif // CRASH_DATA_PROCESSOR_H
