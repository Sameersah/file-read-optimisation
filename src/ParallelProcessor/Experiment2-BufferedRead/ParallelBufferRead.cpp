#include "ParallelBufferRead.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <omp.h>
// #include "../../SequentialProcessor/common/GlobalMutex.h"

// std::mutex records_mutex;

void ProcessorUsingBufferedFileReadThreads::loadData(const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    const size_t BUFFER_SIZE = 10 * 1024 * 1024; // 10MB buffer
    std::vector<char> buffer(BUFFER_SIZE);
    std::vector<std::string> lines;
    std::string remaining_data;

    std::getline(file, remaining_data); // Skip header

    while (file.read(buffer.data(), BUFFER_SIZE) || file.gcount() > 0) {
        std::string chunk(buffer.data(), file.gcount());
        chunk = remaining_data + chunk;

        std::istringstream ss(chunk);
        std::string line;
        remaining_data.clear();

        while (std::getline(ss, line)) {
            if (ss.eof()) {
                remaining_data = line;
                break;
            }
            lines.push_back(line);
        }
    }

    file.close();

    // Process lines in parallel
    processLinesParallel(lines);

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
}

void ProcessorUsingBufferedFileReadThreads::processLinesParallel(const std::vector<std::string>& lines) {
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for parallel processing.\n";

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
        try { record.latitude = token.empty() ? 0.0f : std::stof(token); }
        catch (...) { record.latitude = 0.0f; }

        std::getline(ss, token, ',');
        try { record.longitude = token.empty() ? 0.0f : std::stof(token); }
        catch (...) { record.longitude = 0.0f; }

        std::getline(ss, record.location, ',');
        std::getline(ss, record.on_street_name, ',');
        std::getline(ss, record.cross_street_name, ',');
        std::getline(ss, record.off_street_name, ',');

        std::getline(ss, token, ',');
        try { record.persons_injured = token.empty() ? 0 : std::stoi(token); }
        catch (...) { record.persons_injured = 0; }

        std::getline(ss, token, ',');
        try { record.persons_killed = token.empty() ? 0 : std::stoi(token); }
        catch (...) { record.persons_killed = 0; }

        #pragma omp critical
        {
            std::lock_guard<std::mutex> lock(std::mutex);
            records.push_back(record);
            dateIndex[record.crash_date].push_back(record);
        }
    }
}

int ProcessorUsingBufferedFileReadThreads::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < records.size(); i++) {
        if (records[i].crash_date >= start_date && records[i].crash_date <= end_date) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    date_range_Searching_duration = end - start;
    return crash_count;
}

int ProcessorUsingBufferedFileReadThreads::getCrashesByInjuryCountRange(int min_injuries, int max_injuries) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < records.size(); i++) {
        if (records[i].persons_injured >= min_injuries && records[i].persons_injured <= max_injuries) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    injury_range_Searching_duration = end - start;
    return crash_count;
}

int ProcessorUsingBufferedFileReadThreads::getCrashesByLocationRange(float lat, float lon, float radius) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < records.size(); i++) {
        float dist = std::sqrt(std::pow(records[i].latitude - lat, 2) + std::pow(records[i].longitude - lon, 2));
        if (dist <= radius) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    location_range_Searching_duration = end - start;
    return crash_count;
}

std::chrono::duration<double> ProcessorUsingBufferedFileReadThreads::getDataLoadDuration() const {
    return data_load_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileReadThreads::getDateRangeSearchingDuration() const {
    return date_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileReadThreads::getInjuryRangeSearchingDuration() const {
    return injury_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileReadThreads::getLocationRangeSearchingDuration() const {
    return location_range_Searching_duration;
}
