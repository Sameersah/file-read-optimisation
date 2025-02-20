#ifndef PROCESSOR_USING_THREADS_H
#define PROCESSOR_USING_THREADS_H

#include "../ICrashDataProcessor.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

struct CrashRecord;

class ProcessorUsingThreads : public ICrashDataProcessor {
private:
    std::vector<CrashRecord> records;
    std::unordered_map<std::string, std::vector<CrashRecord>> dateIndex;
    std::chrono::duration<double> data_load_duration = {};
    std::chrono::duration<double> date_range_Searching_duration = {};
    std::chrono::duration<double> injury_range_Searching_duration = {};
    std::chrono::duration<double> location_range_Searching_duration = {};

public:
    // Load data with multi-threading
    void loadData(const std::string& filename) override;

    // Multi-threaded range searching APIs
    [[nodiscard]] std::vector<CrashRecord> getCrashesInDateRange(const std::string& start_date, const std::string& end_date) override;
    [[nodiscard]] std::vector<CrashRecord> getCrashesByInjuryCountRange(int min_injuries, int max_injuries) override;
    [[nodiscard]] std::vector<CrashRecord> getCrashesByLocationRange(float lat, float lon, float radius) override;

    // Get benchmarking results
    [[nodiscard]] std::chrono::duration<double> getDataLoadDuration() const override { return data_load_duration; }
    [[nodiscard]] std::chrono::duration<double> getDateRangeSearchingDuration() const override { return date_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getInjuryRangeSearchingDuration() const override { return injury_range_Searching_duration; }
    [[nodiscard]] std::chrono::duration<double> getLocationRangeSearchingDuration() const override { return location_range_Searching_duration; }
};

#endif // PROCESSOR_USING_THREADS_H
