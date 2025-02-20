#ifndef OPTIMAL_VECTOR_RESERVE_H
#define OPTIMAL_VECTOR_RESERVE_H

#include "../../SequentialProcessor/common/CrashRecord.h"
#include "../../ICrashDataProcessor.h"

#include <vector>
#include <unordered_map>
#include <chrono>
#include <string>

class OptimalVectorReserve : public ICrashDataProcessor {
private:
    std::vector<std::string> crash_dates;
    std::vector<int> persons_injured;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::unordered_map<std::string, std::vector<CrashRecord>> dateIndex;
    
    std::chrono::duration<double> data_load_duration = {};
    std::chrono::duration<double> date_range_Searching_duration = {};
    std::chrono::duration<double> injury_range_Searching_duration = {};
    std::chrono::duration<double> location_range_Searching_duration = {};

    void processLinesParallel(const std::vector<std::string>& lines);

public:
    void loadData(const std::string& filename) override;
    int getCrashesInDateRange(const std::string& start_date, const std::string& end_date) override;
    int getCrashesByInjuryCountRange(int min_injuries, int max_injuries) override;
    int getCrashesByLocationRange(float lat, float lon, float radius) override;

    std::chrono::duration<double> getDataLoadDuration() const override;
    std::chrono::duration<double> getDateRangeSearchingDuration() const override;
    std::chrono::duration<double> getInjuryRangeSearchingDuration() const override;
    std::chrono::duration<double> getLocationRangeSearchingDuration() const override;
};

#endif // OPTIMAL_VECTOR_RESERVE_H
