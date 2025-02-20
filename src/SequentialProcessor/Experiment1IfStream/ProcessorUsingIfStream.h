//
// Created by Sameer on 18/02/25.
//

#ifndef PROCESSOR_USING_IFSTREAM_H
#define PROCESSOR_USING_IFSTREAM_H

#include "../../common/ICrashDataProcessor.h"
#include <unordered_map>
#include <iostream>


struct CrashRecord;

class ProcessorUsingIfStream : public ICrashDataProcessor {
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
     [[nodiscard]] int getCrashesInDateRange(const std::string& start_date, const std::string& end_date) override;
     [[nodiscard]] int getCrashesByInjuryCountRange(int min_injuries, int max_injuries)  override;
     [[nodiscard]] int getCrashesByLocationRange(float lat, float lon, float radius) override;

    //getters
    [[nodiscard]] std::chrono::duration<double> getDataLoadDuration() const override { return data_load_duration; }
    [[nodiscard]] std::chrono::duration<double> getDateRangeSearchingDuration() const override { return date_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getInjuryRangeSearchingDuration() const override { return injury_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getLocationRangeSearchingDuration() const override { return location_range_Searching_duration; }

};

#endif // PROCESSOR_USING_IFSTREAM_H
