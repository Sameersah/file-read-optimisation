#include "ProcessorUsingPartialRead.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <omp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <omp.h>




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

    // Process file in parallel
    processFileParallel(data, file_size);

    // Cleanup
    munmap(data, file_size);
    close(fd);

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
}

void ProcessorUsingPartialRead::processFileParallel(char* data, size_t file_size) {
    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for parallel processing.\n";

    // Thread-local buffers
    std::vector<std::vector<std::string>> crash_dates_local(num_threads);
    std::vector<std::vector<int>> persons_injured_local(num_threads);
    std::vector<std::vector<float>> latitudes_local(num_threads);
    std::vector<std::vector<float>> longitudes_local(num_threads);

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

            while (pos < line.size()) {
                size_t next_pos = line.find(',', pos);
                if (next_pos == std::string::npos) next_pos = line.size();
                token = line.substr(pos, next_pos - pos);
                pos = next_pos + 1;

                switch (col_index) {
                    case 0: crash_date = std::string(token); break;  // crash_date
                    case 4: try { lat = std::stof(std::string(token)); } catch (...) { lat = 0.0f; } break; // latitude
                    case 5: try { lon = std::stof(std::string(token)); } catch (...) { lon = 0.0f; } break; // longitude
                    case 10: try { injured = std::stoi(std::string(token)); } catch (...) { injured = 0; } break; // persons_injured
                }

                col_index++;
                if (col_index > 10) break; // Only process required columns
            }

            local_crash_dates.push_back(crash_date);
            local_latitudes.push_back(lat);
            local_longitudes.push_back(lon);
            local_persons_injured.push_back(injured);
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






int ProcessorUsingPartialRead::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
    auto start = std::chrono::high_resolution_clock::now();
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
