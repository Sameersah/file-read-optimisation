#include "OptimalProcessorUsingThreads.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>
#include <omp.h>

void OptimalProcessorUsingThreads::loadData(const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for parallel data loading.\n";

    #pragma omp parallel for num_threads(num_threads)
    for (size_t i = 0; i < lines.size(); i++) {
        std::istringstream ss(lines[i]);
        std::string token;
        CrashRecord record;

        std::getline(ss, record.crash_date, ',');
        std::getline(ss, record.crash_time, ',');
        std::getline(ss, record.borough, ',');
        std::getline(ss, record.zip_code, ',');

        std::getline(ss, token, ',');
        record.latitude = token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        record.longitude = token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, record.location, ',');
        std::getline(ss, record.on_street_name, ',');
        std::getline(ss, record.cross_street_name, ',');
        std::getline(ss, record.off_street_name, ',');

        std::getline(ss, token, ',');
        try { record.persons_injured = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.persons_injured = 0; }

        #pragma omp critical
        {
            // records.push_back(record);
            crash_dates.push_back(record.crash_date);
            persons_injured.push_back(record.persons_injured);
            latitudes.push_back(record.latitude);
            longitudes.push_back(record.longitude);
            // dateIndex[record.crash_date].push_back(record);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
    std::cout << "Parallel Data Load Time: " << data_load_duration.count() << " seconds\n";
}

// Multi-threaded search for date range
int OptimalProcessorUsingThreads::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
    auto start = std::chrono::high_resolution_clock::now();
    // std::vector<CrashRecord> filtered_crashes;
    int crash_count = 0;
    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < crash_dates.size(); i++) {
        if (crash_dates[i] >= start_date && crash_dates[i] <= end_date) {
            crash_count++;
        }
    }


    auto end = std::chrono::high_resolution_clock::now();
    date_range_Searching_duration = end - start;
    return crash_count;
}

// Multi-threaded search for injury count range
int OptimalProcessorUsingThreads::getCrashesByInjuryCountRange(int min_injuries, int max_injuries) {
    auto start = std::chrono::high_resolution_clock::now();
    // std::vector<CrashRecord> filtered_crashes;
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < persons_injured.size(); i++) {
        if (persons_injured[i] >= min_injuries && persons_injured[i] <= max_injuries) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    injury_range_Searching_duration = end - start;
    return crash_count;
}

// Multi-threaded search for location range
int OptimalProcessorUsingThreads::getCrashesByLocationRange(float lat, float lon, float radius) {
    auto start = std::chrono::high_resolution_clock::now();
    // std::vector<CrashRecord> filtered_crashes;
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < latitudes.size(); i++) {
        float dist = std::sqrt(std::pow(latitudes[i] - lat, 2) + std::pow(longitudes[i] - lon, 2));
        if (dist <= radius) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    location_range_Searching_duration = end - start;
    return crash_count;
}
