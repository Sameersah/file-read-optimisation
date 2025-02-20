#include "../common/CrashRecord.h"
#include "ProcessorUsingBufferedFileRead.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <mutex>


void ProcessorUsingBufferedFileRead::loadData(const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();

    // Open the file in binary mode for efficient reading
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    const size_t BUFFER_SIZE = 10 * 1024 * 1024; // 10MB buffer
    std::vector<char> buffer(BUFFER_SIZE);
    std::string remaining_data; // To handle cases where a line is split between buffers

    // Reserve space for records to avoid multiple allocations
    records.reserve(2000000);
    // Read the header line and discard
    std::getline(file, remaining_data);

    while (file.read(buffer.data(), BUFFER_SIZE) || file.gcount() > 0) {
        std::string chunk(buffer.data(), file.gcount());
        chunk = remaining_data + chunk; // Prepend any leftover data from the last buffer

        std::istringstream ss(chunk);
        std::string line;
        remaining_data.clear();

        while (std::getline(ss, line)) {
            if (ss.eof()) {
                remaining_data = line; // Save the last incomplete line
                break;
            }
            processLine(line);
        }
    }

    file.close();
    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
}

void ProcessorUsingBufferedFileRead::processLine(const std::string& line) {
    std::istringstream ss(line);
    std::string token;
    CrashRecord record;

    std::getline(ss, record.crash_date, ',');
    std::getline(ss, record.crash_time, ',');
    std::getline(ss, record.borough, ',');
    std::getline(ss, record.zip_code, ',');

    std::getline(ss, token, ',');
    //record.latitude = token.empty() ? 0.0f : std::stof(token);
    try {
        record.latitude = token.empty() ? 0.0f : std::stof(token);
    } catch (const std::invalid_argument& e) {
        record.latitude = 0.0f;
    }
    std::getline(ss, token, ',');
    try {
        record.longitude = token.empty() ? 0.0f : std::stof(token);
    } catch (const std::invalid_argument& e) {
        record.longitude = 0.0f;
    }
    //record.longitude = token.empty() ? 0.0f : std::stof(token);

    std::getline(ss, record.location, ',');
    std::getline(ss, record.on_street_name, ',');
    std::getline(ss, record.cross_street_name, ',');
    std::getline(ss, record.off_street_name, ',');

    std::getline(ss, token, ',');
    try {
        record.persons_injured = token.empty() ? 0 : std::stoi(token);
    } catch (...) {
        record.persons_injured = 0;
    }

    std::getline(ss, token, ',');
    try {
        record.persons_killed = token.empty() ? 0 : std::stoi(token);
    } catch (...) {
        record.persons_killed = 0;
    }

    std::getline(ss, record.contributing_factor_vehicle_1, ',');
    std::getline(ss, record.contributing_factor_vehicle_2, ',');
    std::getline(ss, record.contributing_factor_vehicle_3, ',');
    std::getline(ss, record.contributing_factor_vehicle_4, ',');
    std::getline(ss, record.contributing_factor_vehicle_5, ',');

    std::getline(ss, token, ',');
    try {
        record.collision_id = token.empty() ? 0 : std::stol(token);
    } catch (...) {
        record.collision_id = 0;
    }

    std::getline(ss, record.vehicle_type_code_1, ',');
    std::getline(ss, record.vehicle_type_code_2, ',');
    std::getline(ss, record.vehicle_type_code_3, ',');
    std::getline(ss, record.vehicle_type_code_4, ',');
    std::getline(ss, record.vehicle_type_code_5, ',');

    // Thread-safe insertion
    {
        std::lock_guard<std::mutex> lock(std::mutex);
        records.push_back(record);
        dateIndex[record.crash_date].push_back(record);
    }
}


int ProcessorUsingBufferedFileRead::getCrashesInDateRange(const std::string& start_date, const std::string& end_date)  {
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
    for (const auto& record : records) {
        std::tm record_tm = {};
        std::istringstream ss_record(record.crash_date);
        ss_record >> std::get_time(&record_tm, "%m/%d/%Y");

        if (ss_record.fail()) {
            std::cerr << "Skipping invalid date format in record: " << record.crash_date << std::endl;
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

int ProcessorUsingBufferedFileRead::getCrashesByInjuryCountRange(int min_injuries, int max_injuries)  {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;
    for (const auto& record : records) {
        if (record.persons_injured >= min_injuries && record.persons_injured <= max_injuries) {
            crash_count++;
        }
    }
    // Benchmarking
    auto end = std::chrono::high_resolution_clock::now();
    injury_range_Searching_duration = end - start;
    return crash_count;
}

int ProcessorUsingBufferedFileRead::getCrashesByLocationRange(float lat, float lon, float radius)  {
    return {};
}


std::chrono::duration<double> ProcessorUsingBufferedFileRead::getDataLoadDuration() const {
    return data_load_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileRead::getDateRangeSearchingDuration() const {
    return date_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileRead::getInjuryRangeSearchingDuration() const {
    return injury_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileRead::getLocationRangeSearchingDuration() const {
    return location_range_Searching_duration;
}
