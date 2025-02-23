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
#include "../../MemoryUsage.h"

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
    MemoryUsage::printMemoryUsage("ProcessorUsingThreadLocalBuffer");
}

void ProcessorUsingThreadLocalBuffer::processLinesParallel(const std::vector<std::string>& lines) {
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for parallel processing.\n";

    // Thread-local buffers
    std::vector<std::vector<std::string>> crash_dates_local(num_threads);
    std::vector<std::vector<int>> persons_injured_local(num_threads);
    std::vector<std::vector<float>> latitudes_local(num_threads);
    std::vector<std::vector<float>> longitudes_local(num_threads);
    std::vector<std::vector<std::string>> crash_time_local(num_threads);
    std::vector<std::vector<std::string>> borough_local(num_threads);
    std::vector<std::vector<std::string>> zip_code_local(num_threads);
    std::vector<std::vector<std::string>> locations_local(num_threads);
    std::vector<std::vector<std::string>> on_street_name_local(num_threads);
    std::vector<std::vector<std::string>> cross_street_name_local(num_threads);
    std::vector<std::vector<std::string>> off_street_name_local(num_threads);
    std::vector<std::vector<std::string>> contributing_factor_vehicle_1_local(num_threads);
    std::vector<std::vector<std::string>> contributing_factor_vehicle_2_local(num_threads);
    std::vector<std::vector<std::string>> contributing_factor_vehicle_3_local(num_threads);
    std::vector<std::vector<std::string>> contributing_factor_vehicle_4_local(num_threads);
    std::vector<std::vector<std::string>> contributing_factor_vehicle_5_local(num_threads);
    std::vector<std::vector<long>> collision_ids_local(num_threads);
    std::vector<std::vector<std::string>> vehicle_type_code_1_local(num_threads);
    std::vector<std::vector<std::string>> vehicle_type_code_2_local(num_threads);
    std::vector<std::vector<std::string>> vehicle_type_code_3_local(num_threads);
    std::vector<std::vector<std::string>> vehicle_type_code_4_local(num_threads);
    std::vector<std::vector<std::string>> vehicle_type_code_5_local(num_threads);
    std::vector<std::vector<std::string>> vehicle_type_code_6_local(num_threads);


    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        auto& local_crash_dates = crash_dates_local[thread_id];
        auto& local_persons_injured = persons_injured_local[thread_id];
        auto& local_latitudes = latitudes_local[thread_id];
        auto& local_longitudes = longitudes_local[thread_id];
        auto& local_crash_time = crash_time_local[thread_id];
        auto& local_borough = borough_local[thread_id];
        auto& local_zip_code = zip_code_local[thread_id];
        auto& local_locations = locations_local[thread_id];
        auto& local_on_street_name = on_street_name_local[thread_id];
        auto& local_cross_street_name = cross_street_name_local[thread_id];
        auto& local_off_street_name = off_street_name_local[thread_id];
        auto& local_contributing_factor_vehicle_1 = contributing_factor_vehicle_1_local[thread_id];
        auto& local_contributing_factor_vehicle_2 = contributing_factor_vehicle_2_local[thread_id];
        auto& local_contributing_factor_vehicle_3 = contributing_factor_vehicle_3_local[thread_id];
        auto& local_contributing_factor_vehicle_4 = contributing_factor_vehicle_4_local[thread_id];
        auto& local_contributing_factor_vehicle_5 = contributing_factor_vehicle_5_local[thread_id];
        auto& local_collision_ids = collision_ids_local[thread_id];
        auto& local_vehicle_type_code_1 = vehicle_type_code_1_local[thread_id];
        auto& local_vehicle_type_code_2 = vehicle_type_code_2_local[thread_id];
        auto& local_vehicle_type_code_3 = vehicle_type_code_3_local[thread_id];
        auto& local_vehicle_type_code_4 = vehicle_type_code_4_local[thread_id];
        auto& local_vehicle_type_code_5 = vehicle_type_code_5_local[thread_id];
        auto& local_vehicle_type_code_6 = vehicle_type_code_6_local[thread_id];


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

            // Store data in thread-local vectors
            local_crash_dates.push_back(record.crash_date);
            local_persons_injured.push_back(record.persons_injured);
            local_latitudes.push_back(record.latitude);
            local_longitudes.push_back(record.longitude);
            local_crash_time.push_back(record.crash_time);
            local_borough.push_back(record.borough);
            local_zip_code.push_back(record.zip_code);
            local_locations.push_back(record.location);
            local_on_street_name.push_back(record.on_street_name);
            local_cross_street_name.push_back(record.cross_street_name);
            local_off_street_name.push_back(record.off_street_name);
            local_contributing_factor_vehicle_1.push_back(record.contributing_factor_vehicle_1);
            local_contributing_factor_vehicle_2.push_back(record.contributing_factor_vehicle_2);
            local_contributing_factor_vehicle_3.push_back(record.contributing_factor_vehicle_3);
            local_contributing_factor_vehicle_4.push_back(record.contributing_factor_vehicle_4);
            local_contributing_factor_vehicle_5.push_back(record.contributing_factor_vehicle_5);
            local_collision_ids.push_back(record.collision_id);
            local_vehicle_type_code_1.push_back(record.vehicle_type_code_1);
            local_vehicle_type_code_2.push_back(record.vehicle_type_code_2);
            local_vehicle_type_code_3.push_back(record.vehicle_type_code_3);
            local_vehicle_type_code_4.push_back(record.vehicle_type_code_4);
            local_vehicle_type_code_5.push_back(record.vehicle_type_code_5);
            local_vehicle_type_code_6.push_back(record.vehicle_type_code_6);

        }
    }

    // Merge thread-local buffers into global vectors
    for (int i = 0; i < num_threads; i++) {
        crash_dates.insert(crash_dates.end(), crash_dates_local[i].begin(), crash_dates_local[i].end());
        persons_injured.insert(persons_injured.end(), persons_injured_local[i].begin(), persons_injured_local[i].end());
        latitudes.insert(latitudes.end(), latitudes_local[i].begin(), latitudes_local[i].end());
        longitudes.insert(longitudes.end(), longitudes_local[i].begin(), longitudes_local[i].end());
        crash_time.insert(crash_time.end(), crash_time_local[i].begin(), crash_time_local[i].end());
        borough.insert(borough.end(), borough_local[i].begin(), borough_local[i].end());
        zip_code.insert(zip_code.end(), zip_code_local[i].begin(), zip_code_local[i].end());
        locations.insert(locations.end(), locations_local[i].begin(), locations_local[i].end());
        on_street_name.insert(on_street_name.end(), on_street_name_local[i].begin(), on_street_name_local[i].end());
        cross_street_name.insert(cross_street_name.end(), cross_street_name_local[i].begin(), cross_street_name_local[i].end());
        off_street_name.insert(off_street_name.end(), off_street_name_local[i].begin(), off_street_name_local[i].end());
        contributing_factor_vehicle_1.insert(contributing_factor_vehicle_1.end(), contributing_factor_vehicle_1_local[i].begin(), contributing_factor_vehicle_1_local[i].end());
        contributing_factor_vehicle_2.insert(contributing_factor_vehicle_2.end(), contributing_factor_vehicle_2_local[i].begin(), contributing_factor_vehicle_2_local[i].end());
        contributing_factor_vehicle_3.insert(contributing_factor_vehicle_3.end(), contributing_factor_vehicle_3_local[i].begin(), contributing_factor_vehicle_3_local[i].end());
        contributing_factor_vehicle_4.insert(contributing_factor_vehicle_4.end(), contributing_factor_vehicle_4_local[i].begin(), contributing_factor_vehicle_4_local[i].end());
        contributing_factor_vehicle_5.insert(contributing_factor_vehicle_5.end(), contributing_factor_vehicle_5_local[i].begin(), contributing_factor_vehicle_5_local[i].end());
        collision_ids.insert(collision_ids.end(), collision_ids_local[i].begin(), collision_ids_local[i].end());
        vehicle_type_code_1.insert(vehicle_type_code_1.end(), vehicle_type_code_1_local[i].begin(), vehicle_type_code_1_local[i].end());
        vehicle_type_code_2.insert(vehicle_type_code_2.end(), vehicle_type_code_2_local[i].begin(), vehicle_type_code_2_local[i].end());
        vehicle_type_code_3.insert(vehicle_type_code_3.end(), vehicle_type_code_3_local[i].begin(), vehicle_type_code_3_local[i].end());
        vehicle_type_code_4.insert(vehicle_type_code_4.end(), vehicle_type_code_4_local[i].begin(), vehicle_type_code_4_local[i].end());
        vehicle_type_code_5.insert(vehicle_type_code_5.end(), vehicle_type_code_5_local[i].begin(), vehicle_type_code_5_local[i].end());
        vehicle_type_code_6.insert(vehicle_type_code_6.end(), vehicle_type_code_6_local[i].begin(), vehicle_type_code_6_local[i].end());

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
