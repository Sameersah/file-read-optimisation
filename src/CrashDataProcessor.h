//
// Created by Sameer on 18/02/25.
//

#ifndef CRASH_DATA_PROCESSOR_H
#define CRASH_DATA_PROCESSOR_H

#include "ICrashDataProcessor.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cmath>

class CrashDataProcessor : public ICrashDataProcessor {
private:
    std::vector<CrashRecord> records;
    std::unordered_map<std::string, std::vector<CrashRecord>> dateIndex;

public:
    void loadData(const std::string& filename) override;
    std::vector<CrashRecord> searchByDate(const std::string& date) const override;
    std::vector<CrashRecord> searchByLocation(float lat, float lon, float radius) const override;
    void displayStatistics() const override;
};

#endif // CRASH_DATA_PROCESSOR_H
