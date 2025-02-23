#include "ProcessorUsingPartialRead.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <omp.h>
#include <fcntl.h>
#include <sstream>
#include <sys/mman.h>
#include <unistd.h>
#include "../../MemoryUsage.h"





void ProcessorUsingPartialRead::loadData(const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();

    // Open the file
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Get file size
    size_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        std::cerr << "Error getting file size" << std::endl;
        close(fd);
        return;
    }

    // Memory-map the file
    char* data = static_cast<char*>(mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (data == MAP_FAILED) {
        std::cerr << "Error memory-mapping file" << std::endl;
        close(fd);
        return;
    }

    // Estimate record count based on file size (assumption: ~100 bytes per record)
    size_t estimated_records = file_size / 100;
    crash_dates.reserve(estimated_records);
    persons_injured.reserve(estimated_records);
    latitudes.reserve(estimated_records);
    longitudes.reserve(estimated_records);
    crash_time.reserve(estimated_records);
    borough.reserve(estimated_records);
    zip_code.reserve(estimated_records);
    locations.reserve(estimated_records);
    on_street_name.reserve(estimated_records);
    cross_street_name.reserve(estimated_records);
    off_street_name.reserve(estimated_records);
    contributing_factor_vehicle_1.reserve(estimated_records);
    contributing_factor_vehicle_2.reserve(estimated_records);
    contributing_factor_vehicle_3.reserve(estimated_records);
    contributing_factor_vehicle_4.reserve(estimated_records);
    contributing_factor_vehicle_5.reserve(estimated_records);
    collision_ids.reserve(estimated_records);
    vehicle_type_code_1.reserve(estimated_records);
    vehicle_type_code_2.reserve(estimated_records);
    vehicle_type_code_3.reserve(estimated_records);
    vehicle_type_code_4.reserve(estimated_records);
    vehicle_type_code_5.reserve(estimated_records);
    vehicle_type_code_6.reserve(estimated_records);



    // Process file in parallel
    processFileParallel(data, file_size);

    // Cleanup
    munmap(data, file_size);
    close(fd);

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
    MemoryUsage::printMemoryUsage("ProcessorUsingPartialRead");
}

void ProcessorUsingPartialRead::processFileParallel(char* data, size_t file_size) {
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


    size_t chunk_size = file_size / num_threads;

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        char* start_pos = data + (thread_id * chunk_size);
        char* end_pos = (thread_id == num_threads - 1) ? data + file_size : start_pos + chunk_size;

        // Ensure each thread starts at the beginning of a new line
        if (thread_id != 0) {
            while (*start_pos != '\n' && start_pos < end_pos) {
                start_pos++;
            }
            if (start_pos < end_pos) {
                start_pos++;
            }
        }

        char* line_start = start_pos;
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


        while (line_start < end_pos) {
            char* line_end = line_start;
            while (line_end < end_pos && *line_end != '\n') {
                line_end++;
            }
            if (line_end >= end_pos) break;

            std::string_view line(line_start, line_end - line_start);
            line_start = line_end + 1; // Move to next line

            size_t col_index = 0;
            size_t pos = 0;
            std::string_view token;
            float lat = 0.0f, lon = 0.0f;
            int injured = 0;
            std::string crash_date;
            std::string crash_time;
            std::string borough;
            std:: string zip_code;
            std::string locations;
            std::string on_street_name;
            std::string cross_street_name;
            std::string off_street_name;
            int number_of_persons_injured = 0;
            int number_of_persons_killed = 0;
            int number_of_pedestrians_injured = 0;
            int number_of_pedestrians_killed = 0;
            int number_of_cyclists_injured = 0;
            int number_of_cyclists_killed = 0;
            int number_of_motorists_injured = 0;
            int number_of_motorists_killed = 0;
            std::string contributing_factor_vehicle_1;
            std::string contributing_factor_vehicle_2;
            std::string contributing_factor_vehicle_3;
            std::string contributing_factor_vehicle_4;
            std::string contributing_factor_vehicle_5;
            std::string vehicle_type_code_1;
            std::string vehicle_type_code_2;
            std::string vehicle_type_code_3;
            std:: string vehicle_type_code_4;
            std::string vehicle_type_code_5;
            std::string vehicle_type_code_6;
            long collision_id = 0;

            while (pos < line.size()) {
                size_t next_pos = line.find(',', pos);
                if (next_pos == std::string::npos) next_pos = line.size();
                token = line.substr(pos, next_pos - pos);
                pos = next_pos + 1;

                switch (col_index) {
                    case 0: crash_date = std::string(token); break;
                    case 1: crash_time = std::string(token); break;
                    case 2: borough = std::string(token); break;
                    case 3: zip_code = std::string(token); break;
                    case 4: try { lat = std::stof(std::string(token)); } catch (...) { lat = 0.0f; } break;
                    case 5: try { lon = std::stof(std::string(token)); } catch (...) { lon = 0.0f; } break;
                    case 6: locations = std::string(token); break;
                    case 7: on_street_name = std::string(token); break;
                    case 8: cross_street_name = std::string(token); break;
                    case 9: off_street_name = std::string(token); break;
                    case 10: try {
                        number_of_persons_injured = std::stoi(std::string(token));
                    } catch (...) {
                        number_of_persons_injured = 0;
                    } break;
                    case 11: try { number_of_persons_killed = std::stoi(std::string(token)); } catch (...) { number_of_persons_killed = 0; } break;
                    case 12: try { number_of_pedestrians_injured = std::stoi(std::string(token)); } catch (...) { number_of_pedestrians_injured = 0; } break;
                    case 13: try { number_of_pedestrians_killed = std::stoi(std::string(token)); } catch (...) { number_of_pedestrians_killed = 0; } break;
                    case 14: try { number_of_cyclists_injured = std::stoi(std::string(token)); } catch (...) { number_of_cyclists_injured = 0; } break;
                    case 15: try { number_of_cyclists_killed = std::stoi(std::string(token)); } catch (...) { number_of_cyclists_killed = 0; } break;
                    case 16: try { number_of_motorists_injured = std::stoi(std::string(token)); } catch (...) { number_of_motorists_injured = 0; } break;
                    case 17: try { number_of_motorists_killed = std::stoi(std::string(token)); } catch (...) { number_of_motorists_killed = 0; } break;
                    case 18: contributing_factor_vehicle_1 = std::string(token); break;
                    case 19: contributing_factor_vehicle_2 = std::string(token); break;
                    case 20: contributing_factor_vehicle_3 = std::string(token); break;
                    case 21: contributing_factor_vehicle_4 = std::string(token); break;
                    case 22: contributing_factor_vehicle_5 = std::string(token); break;
                    case 23: try { collision_id = std::stol(std::string(token)); } catch (...) { collision_id = 0; } break;
                    case 24: vehicle_type_code_1 = std::string(token); break;
                    case 25: vehicle_type_code_2 = std::string(token); break;
                    case 26: vehicle_type_code_3 = std::string(token); break;
                    case 27: vehicle_type_code_4 = std::string(token); break;
                    case 28: vehicle_type_code_5 = std::string(token); break;
                    case 29: vehicle_type_code_6 = std::string(token); break;
                    case 30: try { injured = std::stoi(std::string(token)); } catch (...) { injured = 0; } break;
                }

                col_index++;
                if (col_index > 30) break; // Only process required columns
            }

            local_crash_dates.push_back(crash_date);
            local_latitudes.push_back(lat);
            local_longitudes.push_back(lon);
            local_persons_injured.push_back(injured);
            local_crash_time.push_back(crash_time);
            local_borough.push_back(borough);
            local_zip_code.push_back(zip_code);
            local_locations.push_back(locations);
            local_on_street_name.push_back(on_street_name);
            local_cross_street_name.push_back(cross_street_name);
            local_off_street_name.push_back(off_street_name);
            local_contributing_factor_vehicle_1.push_back(contributing_factor_vehicle_1);
            local_contributing_factor_vehicle_2.push_back(contributing_factor_vehicle_2);
            local_contributing_factor_vehicle_3.push_back(contributing_factor_vehicle_3);
            local_contributing_factor_vehicle_4.push_back(contributing_factor_vehicle_4);
            local_contributing_factor_vehicle_5.push_back(std::string(contributing_factor_vehicle_5));
            local_collision_ids.push_back(collision_id);
            local_vehicle_type_code_1.push_back(std::string(vehicle_type_code_1));
            local_vehicle_type_code_2.push_back(std::string(vehicle_type_code_2));
            local_vehicle_type_code_3.push_back(std::string(vehicle_type_code_3));
            local_vehicle_type_code_4.push_back(std::string(vehicle_type_code_4));
            local_vehicle_type_code_5.push_back(std::string(vehicle_type_code_5));
            local_vehicle_type_code_6.push_back(std::string(vehicle_type_code_6));

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






int ProcessorUsingPartialRead::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
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

int ProcessorUsingPartialRead::getCrashesByInjuryCountRange(int min_injuries, int max_injuries) {
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

int ProcessorUsingPartialRead::getCrashesByLocationRange(float lat, float lon, float radius) {
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

std::chrono::duration<double> ProcessorUsingPartialRead::getDataLoadDuration() const {
    return data_load_duration;
}

std::chrono::duration<double> ProcessorUsingPartialRead::getDateRangeSearchingDuration() const {
    return date_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingPartialRead::getInjuryRangeSearchingDuration() const {
    return injury_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingPartialRead::getLocationRangeSearchingDuration() const {
    return location_range_Searching_duration;
}
