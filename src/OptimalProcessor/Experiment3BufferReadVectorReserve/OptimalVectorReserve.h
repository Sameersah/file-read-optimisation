#ifndef OPTIMAL_VECTOR_RESERVE_H
#define OPTIMAL_VECTOR_RESERVE_H


#include "../../common/ICrashDataProcessor.h"

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

    std::vector<std::string> crash_time;
    std::vector<std::string> borough;
    std::vector<std::string> zip_code;
    std::vector<std::string> locations;
    std::vector<std::string> on_street_name;
    std::vector<std::string> cross_street_name;
    std::vector<std::string> off_street_name;
    std::vector<std::string> contributing_factor_vehicle_1;
    std::vector<std::string> contributing_factor_vehicle_2;
    std::vector<std::string> contributing_factor_vehicle_3;
    std::vector<std::string> contributing_factor_vehicle_4;
    std::vector<std::string> contributing_factor_vehicle_5;
    std::vector<long> collision_ids;
    std::vector<std::string> vehicle_type_code_1;
    std::vector<std::string> vehicle_type_code_2;
    std::vector<std::string> vehicle_type_code_3;
    std::vector<std::string> vehicle_type_code_4;
    std::vector<std::string> vehicle_type_code_5;
    std::vector<std::string> vehicle_type_code_6;
    std::vector<std::string> vehicle_type_1;
    std::vector<std::string> vehicle_type_2;
    std::vector<std::string> vehicle_type_3;
    std::vector<std::string> vehicle_type_4;
    std::vector<std::string> vehicle_type_5;
    std::vector<std::string> vehicle_type_6;
    
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
