#include "OptimalProcessorUsingThreads.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>
#include <omp.h>
#include "../../MemoryUsage.h"

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

        try { record.cyclists_injured = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.cyclists_injured = 0; }

        try { record.cyclists_killed = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.cyclists_killed = 0; }

        try { record.motorists_injured = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.motorists_injured = 0; }

        try { record.motorists_killed = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.motorists_killed = 0; }

        std::getline(ss, record.contributing_factor_vehicle_1, ',');
        std::getline(ss, record.contributing_factor_vehicle_2, ',');
        std::getline(ss, record.contributing_factor_vehicle_3, ',');
        std::getline(ss, record.contributing_factor_vehicle_4, ',');
        std::getline(ss, record.contributing_factor_vehicle_5, ',');

        try { record.collision_id= token.empty() ? 0 : std::stol(token); }
        catch (const std::exception&) { record.collision_id = 0; }
        std::getline(ss, record.vehicle_type_code_1, ',');
        std::getline(ss, record.vehicle_type_code_2, ',');
        std::getline(ss, record.vehicle_type_code_3, ',');
        std::getline(ss, record.vehicle_type_code_4, ',');
        std::getline(ss, record.vehicle_type_code_5, ',');

        #pragma omp critical
        {
            // records.push_back(record);
            crash_dates.push_back(record.crash_date);
            persons_injured.push_back(record.persons_injured);
            latitudes.push_back(record.latitude);
            longitudes.push_back(record.longitude);
            crash_time.push_back(record.crash_date);
            borough.push_back(record.borough);
            zip_code.push_back(record.zip_code);
            locations.push_back(record.location);
            on_street_name.push_back(record.on_street_name);
            cross_street_name.push_back(record.cross_street_name);
            off_street_name.push_back(record.off_street_name);
            contributing_factor_vehicle_1.push_back(record.contributing_factor_vehicle_1);
            contributing_factor_vehicle_2.push_back(record.contributing_factor_vehicle_2);
            contributing_factor_vehicle_3.push_back(record.contributing_factor_vehicle_3);
            contributing_factor_vehicle_4.push_back(record.contributing_factor_vehicle_4);
            contributing_factor_vehicle_5.push_back(record.contributing_factor_vehicle_5);
            collision_ids.push_back(record.collision_id);
            vehicle_type_code_1.push_back(record.vehicle_type_code_1);
            vehicle_type_code_2.push_back(record.vehicle_type_code_2);
            vehicle_type_code_3.push_back(record.vehicle_type_code_3);
            vehicle_type_code_4.push_back(record.vehicle_type_code_4);
            vehicle_type_code_5.push_back(record.vehicle_type_code_5);
            vehicle_type_code_6.push_back(record.vehicle_type_code_6);


        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
    std::cout << "Parallel Data Load Time: " << data_load_duration.count() << " seconds\n";
    MemoryUsage::printMemoryUsage("OptimalProcessorUsingThreads");
}

// Multi-threaded search for date range
int OptimalProcessorUsingThreads::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
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
