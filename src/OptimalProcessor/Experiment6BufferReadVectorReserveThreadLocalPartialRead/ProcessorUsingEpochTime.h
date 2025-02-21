#ifndef PROCESSOR_USING_EPOCH_TIME_H
#define PROCESSOR_USING_EPOCH_TIME_H

#include "../../common/CrashRecord.h"
#include "../../common/ICrashDataProcessor.h"

#include <vector>
#include <unordered_map>
#include <chrono>
#include <string>

class ProcessorUsingEpochTime : public ICrashDataProcessor {
private:
    std::vector<time_t> crash_dates_epoch;
     std::vector<std::string> crash_dates;
    std::vector<int> persons_injured;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    
    std::chrono::duration<double> data_load_duration = {};
    std::chrono::duration<double> date_range_Searching_duration = {};
    std::chrono::duration<double> injury_range_Searching_duration = {};
    std::chrono::duration<double> location_range_Searching_duration = {};

    void processLinesParallel(const std::vector<std::string>& lines);
    void processFileParallel(char* data, size_t file_size);

public:
    ProcessorUsingEpochTime();
    void loadData(const std::string& filename) override;
    int getCrashesInDateRange(const std::string& start_date, const std::string& end_date) override;
    int getCrashesByInjuryCountRange(int min_injuries, int max_injuries) override;
    int getCrashesByLocationRange(float lat, float lon, float radius) override;

    std::chrono::duration<double> getDataLoadDuration() const override;
    std::chrono::duration<double> getDateRangeSearchingDuration() const override;
    std::chrono::duration<double> getInjuryRangeSearchingDuration() const override;
    std::chrono::duration<double> getLocationRangeSearchingDuration() const override;
};

#endif // PROCESSOR_USING_PARTIAL_READ_H
