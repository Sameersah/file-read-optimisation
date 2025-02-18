#include "CrashDataProcessor.h"
#include <memory>

int main() {
    auto processor = std::make_unique<CrashDataProcessor>();
    processor->loadData("../motor_vehicle_collisions.csv");

    std::cout << "\nCrash Statistics:\n";
    processor->displayStatistics();

    std::cout << "\nSearching for crashes on 2023-01-01:\n";
    auto results = processor->searchByDate("2023-01-01");
    for (const auto& record : results) {
        std::cout << "Crash on " << record.crash_date << " at " << record.crash_time << " with " << record.persons_injured << " injuries." << std::endl;
    }

    return 0;
}