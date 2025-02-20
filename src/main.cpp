#include <iostream>
#include <memory>
#include "SequentialProcessor/IfStreamProcessor/ProcessorUsingIfStream.h"
#include "SequentialProcessor/BufferedFileReadProcessor/ProcessorUsingBufferedFileRead.h"
#include "SequentialProcessor/BufferedFileReadVectorReserveProcessor/ProcessorUsingBufferedFileReadVectorReserve.h"
#include "ParallelProcessor/ProcessorUsingThreads.h"

void runProcessor(std::unique_ptr<ICrashDataProcessor>& processor) {
    std::string filename = "../motor_vehicle_collisions.csv";

    std::cout << "Loading data..." << std::endl;
    processor->loadData(filename);
    std::cout << "Data load duration: " << processor->getDataLoadDuration().count() << " seconds" << std::endl;

    std::string start_date, end_date;
    std::cout << "Enter start date (MM/DD/YYYY): ";
    std::cin >> start_date;
    std::cout << "Enter end date (MM/DD/YYYY): ";
    std::cin >> end_date;

    auto crashes = processor->getCrashesInDateRange(start_date, end_date);
    std::cout << "Number of crashes in date range: " << crashes.size() << std::endl;
    std::cout << "Date range searching duration: " << processor->getDateRangeSearchingDuration().count() << " seconds" << std::endl;

    int min_injuries, max_injuries;
    std::cout << "Enter minimum injuries: ";
    std::cin >> min_injuries;
    std::cout << "Enter maximum injuries: ";
    std::cin >> max_injuries;

    crashes = processor->getCrashesByInjuryCountRange(min_injuries, max_injuries);
    std::cout << "Number of crashes in injury count range: " << crashes.size() << std::endl;
    std::cout << "Injury count range searching duration: " << processor->getInjuryRangeSearchingDuration().count() << " seconds" << std::endl;

    float lat, lon, radius;
    std::cout << "Enter latitude: ";
    std::cin >> lat;
    std::cout << "Enter longitude: ";
    std::cin >> lon;
    std::cout << "Enter radius (km): ";
    std::cin >> radius;

    crashes = processor->getCrashesByLocationRange(lat, lon, radius);
    std::cout << "Number of crashes in location range: " << crashes.size() << std::endl;
    std::cout << "Location range searching duration: " << processor->getLocationRangeSearchingDuration().count() << " seconds" << std::endl;
}

int main() {
    while (true) {
        std::cout << "\n=============== Crash Data Processing ===============\n";
        std::cout << "1. Single Thread (ifstream)\n";
        std::cout << "2. Buffered File Read\n";
        std::cout << "3. Buffered File Read and Vector memory reserve\n";
        std::cout << "4. Multi thread\n";
        std::cout << "5. Memory-Mapped File (Coming Soon)\n";
        std::cout << "6. Binary Format Processing (Coming Soon)\n";
        std::cout << "7. Exit\n";
        std::cout << "=====================================================\n";
        std::cout << "Select processing method: ";

        int choice;
        std::cin >> choice;

        if (choice == 7) {
            std::cout << "Exiting program. Goodbye!\n";
            break;
        }

        std::unique_ptr<ICrashDataProcessor> processor;

        switch (choice) {
            case 1:
                std::cout << "\nUsing Single Threaded Processing (ifstream)...\n";
                processor = std::make_unique<ProcessorUsingIfStream>();
                runProcessor(processor);
                break;

            case 2:
                std::cout << "\nUsing Buffered File Read Processing...\n";
                processor = std::make_unique<ProcessorUsingBufferedFileRead>();
                runProcessor(processor);
                break;

            case 3:
                std::cout << "\nUsing Buffered File Read and Vector reserve Processing...\n";
               processor = std::make_unique<ProcessorUsingBufferedFileReadVectorReserve>();
               runProcessor(processor);
               break;

            case 4:
                std::cout << "\nUsing Multi-threaded Parsing...\n";
                processor = std::make_unique<ProcessorUsingThreads>();
                runProcessor(processor);
                break;

            case 5:
                std::cout << "\nMemory-Mapped File Processing is not implemented yet.\n";
                break;

            case 6:
                std::cout << "\nBinary Format Processing is not implemented yet.\n";
                break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}
