//
// Created by Sameer on 18/02/25.
//

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "ProcessorUsingIfStream.h"


void ProcessorUsingIfStream::loadData(const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        CrashRecord record;

        // Parse each column, handling potential missing values
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
        try {
        record.persons_injured = token.empty() ? 0 : std::stoi(token);
     } catch (const std::invalid_argument& e) {
        record.persons_injured = 0;
    }

        std::getline(ss, token, ',');

        try {record.persons_killed = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.persons_killed = 0;
        }

        std::getline(ss, token, ',');
       try{ record.pedestrians_injured = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.pedestrians_injured = 0;
        }

        std::getline(ss, token, ',');
        try{record.pedestrians_killed = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.pedestrians_killed = 0;
        }

        std::getline(ss, token, ',');
        try{record.cyclists_injured = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.cyclists_injured = 0;
        }

        std::getline(ss, token, ',');
        try{record.cyclists_killed = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.cyclists_killed = 0;
        }

        std::getline(ss, token, ',');
        try{record.motorists_injured = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.motorists_injured = 0;
        }

        std::getline(ss, token, ',');
        try{record.motorists_killed = token.empty() ? 0 : std::stoi(token);}
        catch (const std::invalid_argument& e) {
            record.motorists_killed = 0;
        }

        std::getline(ss, record.contributing_factor_vehicle_1, ',');
        std::getline(ss, record.contributing_factor_vehicle_2, ',');
        std::getline(ss, record.contributing_factor_vehicle_3, ',');
        std::getline(ss, record.contributing_factor_vehicle_4, ',');
        std::getline(ss, record.contributing_factor_vehicle_5, ',');
        std::getline(ss, token, ',');
        try {
            record.collision_id = token.empty() ? 0 : std::stol(token);
        } catch (const std::invalid_argument& e) {
            record.collision_id = 0;
        }
        std::getline(ss, record.vehicle_type_code_1, ',');
        std::getline(ss, record.vehicle_type_code_2, ',');
        std::getline(ss, record.vehicle_type_code_3, ',');
        std::getline(ss, record.vehicle_type_code_4, ',');
        std::getline(ss, record.vehicle_type_code_5, ',');

        records.push_back(record);
        // dateIndex[record.crash_date].push_back(record);
    }

    file.close();

    //benchmarking
    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
}


int ProcessorUsingIfStream::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
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

int ProcessorUsingIfStream::getCrashesByInjuryCountRange(int min_injuries, int max_injuries)  {
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


int ProcessorUsingIfStream::getCrashesByLocationRange(float lat, float lon, float radius)  {
    return {};
}






