#include "ProcessorUsingThreadLocalBuffer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <omp.h>


#include "ProcessorUsingThreadLocalBuffer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <omp.h>

void ProcessorUsingThreadLocalBuffer::loadData(const std::string& filename) {
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

    // Preallocate vector space based on estimated record count
    size_t estimated_records = lines.size();
    crash_dates.reserve(estimated_records);
    persons_injured.reserve(estimated_records);
    latitudes.reserve(estimated_records);
    longitudes.reserve(estimated_records);

    // Process lines in parallel using thread-local buffers
    processLinesParallel(lines);

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
}

void ProcessorUsingThreadLocalBuffer::processLinesParallel(const std::vector<std::string>& lines) {
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for parallel processing.\n";

    // Thread-local buffers
    std::vector<std::vector<std::string>> crash_dates_local(num_threads);
    std::vector<std::vector<int>> persons_injured_local(num_threads);
    std::vector<std::vector<float>> latitudes_local(num_threads);
    std::vector<std::vector<float>> longitudes_local(num_threads);

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        auto& local_crash_dates = crash_dates_local[thread_id];
        auto& local_persons_injured = persons_injured_local[thread_id];
        auto& local_latitudes = latitudes_local[thread_id];
        auto& local_longitudes = longitudes_local[thread_id];

        #pragma omp for
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

            // Store data in thread-local vectors
            local_crash_dates.push_back(record.crash_date);
            local_persons_injured.push_back(record.persons_injured);
            local_latitudes.push_back(record.latitude);
            local_longitudes.push_back(record.longitude);
        }
    }

    // Merge thread-local buffers into global vectors
    for (int i = 0; i < num_threads; i++) {
        crash_dates.insert(crash_dates.end(), crash_dates_local[i].begin(), crash_dates_local[i].end());
        persons_injured.insert(persons_injured.end(), persons_injured_local[i].begin(), persons_injured_local[i].end());
        latitudes.insert(latitudes.end(), latitudes_local[i].begin(), latitudes_local[i].end());
        longitudes.insert(longitudes.end(), longitudes_local[i].begin(), longitudes_local[i].end());
    }
}


int ProcessorUsingThreadLocalBuffer::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;
    std::tm start_tm = {}, end_tm = {};
    std::istringstream ss_start(start_date);
    std::istringstream ss_end(end_date);
    ss_start >> std::get_time(&start_tm, "%m/%d/%Y");
    ss_end >> std::get_time(&end_tm, "%m/%d/%Y");
    if (ss_start.fail() || ss_end.fail()) {
        std::cerr << "Error: Invalid date format (Expected MM/DD/YYYY)" << std::endl;
        return {}; // Return empty if parsing fails
    }
    time_t start_time = std::mktime(&start_tm);
    time_t end_time = std::mktime(&end_tm);

    #pragma omp parallel for reduction(+:crash_count)
    for (const auto& crash_date : crash_dates) {
        std::tm record_tm = {};
        std::istringstream ss_record(crash_date);
        ss_record >> std::get_time(&record_tm, "%m/%d/%Y");

        if (ss_record.fail()) {
            std::cerr << "Skipping invalid date format in record: " << crash_date << std::endl;
            continue; // Skip records with invalid date format
        }

        time_t record_time = std::mktime(&record_tm);

        if (record_time >= start_time && record_time <= end_time) {
            crash_count++;
        }
    }
    //benchmarking
    auto end = std::chrono::high_resolution_clock::now();
    date_range_Searching_duration = end - start;
    return crash_count;
}

int ProcessorUsingThreadLocalBuffer::getCrashesByInjuryCountRange(int min_injuries, int max_injuries) {
    auto start = std::chrono::high_resolution_clock::now();
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

int ProcessorUsingThreadLocalBuffer::getCrashesByLocationRange(float lat, float lon, float radius) {
    auto start = std::chrono::high_resolution_clock::now();
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

std::chrono::duration<double> ProcessorUsingThreadLocalBuffer::getDataLoadDuration() const {
    return data_load_duration;
}

std::chrono::duration<double> ProcessorUsingThreadLocalBuffer::getDateRangeSearchingDuration() const {
    return date_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingThreadLocalBuffer::getInjuryRangeSearchingDuration() const {
    return injury_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingThreadLocalBuffer::getLocationRangeSearchingDuration() const {
    return location_range_Searching_duration;
}
