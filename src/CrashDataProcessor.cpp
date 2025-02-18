//
// Created by Sameer on 18/02/25.
//

#include "CrashDataProcessor.h"

void CrashDataProcessor::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        CrashRecord record;
        std::string temp;

        std::getline(ss, record.crash_date, ',');
        std::getline(ss, record.crash_time, ',');
        std::getline(ss, record.borough, ',');
        std::getline(ss, record.zip_code, ',');

        std::getline(ss, temp, ',');
        try {record.latitude = temp.empty() ? 0.0f : std::stof(temp);}
        catch (const std::exception&) { record.latitude = 0.0f; }

        std::getline(ss, temp, ',');
        try {record.longitude = temp.empty() ? 0.0f : std::stof(temp);}
        catch (const std::exception&) { record.longitude = 0.0f; }

        std::getline(ss, temp, ',');
        try {record.persons_injured = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.persons_injured = 0; }

        std::getline(ss, temp, ',');
        try {record.persons_killed = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.persons_killed = 0; }

        std::getline(ss, temp, ',');
        try {record.pedestrians_injured = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.pedestrians_injured = 0; }

        std::getline(ss, temp, ',');
        try {record.pedestrians_killed = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.pedestrians_killed = 0; }

        std::getline(ss, temp, ',');
        try {record.cyclists_injured = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.cyclists_injured = 0; }

        std::getline(ss, temp, ',');
        try {record.cyclists_killed = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.cyclists_killed = 0; }

        std::getline(ss, temp, ',');
        try {record.motorists_injured = temp.empty() ? 0 : std::stoi(temp); }
        catch (const std::exception&) { record.motorists_injured = 0; }

        std::getline(ss, temp, ',');
        try {record.motorists_killed = temp.empty() ? 0 : std::stoi(temp);}
        catch (const std::exception&) { record.motorists_killed = 0; }

        std::getline(ss, record.contributing_factor_1, ',');
        std::getline(ss, record.contributing_factor_2, ',');
        std::getline(ss, record.vehicle_type_1, ',');
        std::getline(ss, record.vehicle_type_2, ',');

        records.push_back(record);
        dateIndex[record.crash_date].push_back(record);
    }
    file.close();
    std::cout << "Data successfully loaded. Total records: " << records.size() << std::endl;
}

#include "CrashDataProcessor.h"

// Implementation for searchByDate
std::vector<CrashRecord> CrashDataProcessor::searchByDate(const std::string& date) const {
    auto it = dateIndex.find(date);
    return (it != dateIndex.end()) ? it->second : std::vector<CrashRecord>{};
}

// Implementation for searchByLocation
std::vector<CrashRecord> CrashDataProcessor::searchByLocation(float lat, float lon, float radius) const {
    std::vector<CrashRecord> result;
    for (const auto& record : records) {
        float dist = std::sqrt(std::pow(record.latitude - lat, 2) + std::pow(record.longitude - lon, 2));
        if (dist <= radius) {
            result.push_back(record);
        }
    }
    return result;
}

// Implementation for displayStatistics
void CrashDataProcessor::displayStatistics() const {
    long total_crashes = records.size();
    long total_injuries = 0;
    long total_killed = 0;

    for (const auto& record : records) {
        total_injuries += record.persons_injured;
        total_killed += record.persons_killed;
    }

    std::cout << "Total Crashes: " << total_crashes << std::endl;
    std::cout << "Total Injuries: " << total_injuries << std::endl;
    std::cout << "Total Fatalities: " << total_killed << std::endl;
}

