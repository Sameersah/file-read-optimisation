//
// Created by Sameer on 18/02/25.
//

#ifndef PROCESSOR_USING_BUFFERED_FILE_READ_VECTOR_RESERVE_H
#define PROCESSOR_USING_BUFFERED_FILE_READ_VECTOR_RESERVE_H

#include "../../ICrashDataProcessor.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <chrono>

class ProcessorUsingBufferedFileReadVectorReserve : public ICrashDataProcessor {
private:
    std::vector<CrashRecord> records;
    std::unordered_map<std::string, std::vector<CrashRecord>> dateIndex;
    std::chrono::duration<double> data_load_duration = {};
    std::chrono::duration<double> date_range_Searching_duration = {};
    std::chrono::duration<double> injury_range_Searching_duration = {};
    std::chrono::duration<double> location_range_Searching_duration = {};

public:
    ProcessorUsingBufferedFileReadVectorReserve() = default;
    ~ProcessorUsingBufferedFileReadVectorReserve() override = default; // ✅ Ensure proper cleanup

    // Read data
    void loadData(const std::string& filename) override;
    void processLine(const std::string& filename) ;

    // APIs for range-based searching
    [[nodiscard]] int getCrashesInDateRange(const std::string& start_date, const std::string& end_date)  override;
    [[nodiscard]] int getCrashesByInjuryCountRange(int min_injuries, int max_injuries)  override;
    [[nodiscard]] int getCrashesByLocationRange(float lat, float lon, float radius)  override;

    // Getters
    [[nodiscard]] std::chrono::duration<double> getDataLoadDuration() const override;
    [[nodiscard]] std::chrono::duration<double> getDateRangeSearchingDuration() const override;
    [[nodiscard]] std::chrono::duration<double> getInjuryRangeSearchingDuration() const override;
    [[nodiscard]] std::chrono::duration<double> getLocationRangeSearchingDuration() const override;
};

#endif // PROCESSOR_USING_BUFFERED_FILE_READ_VECTOR_RESERVE_H
