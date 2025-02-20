#ifndef OPTIMAL_PROCESSOR_USING_THREADS_H
#define OPTIMAL_PROCESSOR_USING_THREADS_H

#include "../../common/ICrashDataProcessor.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

struct CrashRecord;

class OptimalProcessorUsingThreads : public ICrashDataProcessor {
private:
    std::vector<std::string> crash_dates;
    std::vector<int> persons_injured;
    std::vector<float> latitudes;
    std::vector<float> longitudes;

    std::chrono::duration<double> data_load_duration = {};
    std::chrono::duration<double> date_range_Searching_duration = {};
    std::chrono::duration<double> injury_range_Searching_duration = {};
    std::chrono::duration<double> location_range_Searching_duration = {};

public:
    // Load data with multi-threading
    void loadData(const std::string& filename) override;

    // Multi-threaded range searching APIs
    [[nodiscard]] int getCrashesInDateRange(const std::string& start_date, const std::string& end_date) override;
    [[nodiscard]] int getCrashesByInjuryCountRange(int min_injuries, int max_injuries) override;
    [[nodiscard]] int getCrashesByLocationRange(float lat, float lon, float radius) override;

    // Get benchmarking results
    [[nodiscard]] std::chrono::duration<double> getDataLoadDuration() const override { return data_load_duration; }
    [[nodiscard]] std::chrono::duration<double> getDateRangeSearchingDuration() const override { return date_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getInjuryRangeSearchingDuration() const override { return injury_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getLocationRangeSearchingDuration() const override { return location_range_Searching_duration; }
};

#endif // OPTIMAL_PROCESSOR_USING_THREADS_H
