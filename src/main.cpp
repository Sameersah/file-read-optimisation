#include <iostream>
#include <memory>
#include "SequentialProcessor/IfStreamProcessor/ProcessorUsingIfStream.h"
#include "SequentialProcessor/BufferedFileReadProcessor/ProcessorUsingBufferedFileRead.h"
#include "SequentialProcessor/BufferedFileReadVectorReserveProcessor/ProcessorUsingBufferedFileReadVectorReserve.h"
#include "ParallelProcessor/Experiment1-Threads/ProcessorUsingThreads.h"
#include "ParallelProcessor/Experiment2-BufferedRead/ParallelBufferRead.h"
#include "ParallelProcessor/Experiment3-VectorReserve/ParallelVectorReserve.h"
#include "OptimalProcessor/IfStreamProcessor/OptimalProcessorUsingThreads.h"
#include "OptimalProcessor/Experiment2-BufferRead/OptimalBufferRead.h"
#include "OptimalProcessor/Experiment3-VectorReserve/OptimalVectorReserve.h"

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

    int total_crashes = processor->getCrashesInDateRange(start_date, end_date);
    std::cout << "Number of crashes in date range: " << total_crashes << std::endl;
    std::cout << "Date range searching duration: " << processor->getDateRangeSearchingDuration().count() << " seconds" << std::endl;

    int min_injuries, max_injuries;
    std::cout << "Enter minimum injuries: ";
    std::cin >> min_injuries;
    std::cout << "Enter maximum injuries: ";
    std::cin >> max_injuries;

    total_crashes = processor->getCrashesByInjuryCountRange(min_injuries, max_injuries);
    std::cout << "Number of crashes in injury count range: " << total_crashes << std::endl;
    std::cout << "Injury count range searching duration: " << processor->getInjuryRangeSearchingDuration().count() << " seconds" << std::endl;

    float lat, lon, radius;
    std::cout << "Enter latitude: ";
    std::cin >> lat;
    std::cout << "Enter longitude: ";
    std::cin >> lon;
    std::cout << "Enter radius (km): ";
    std::cin >> radius;

    total_crashes = processor->getCrashesByLocationRange(lat, lon, radius);
    std::cout << "Number of crashes in location range: " << total_crashes << std::endl;
    std::cout << "Location range searching duration: " << processor->getLocationRangeSearchingDuration().count() << " seconds" << std::endl;
}

int main() {
    while (true) {
        std::cout << "\n=============== Crash Data Processing ===============\n";
        std::cout << "1. Single Thread (ifstream)\n";
        std::cout << "2. Buffered File Read\n";
        std::cout << "3. Buffered File Read and Vector memory reserve\n";
        std::cout << "4. Multi thread\n";
        std::cout << "5. Multi thread Buffered File Read\n";
        std::cout << "6. Multi thread Buffered File Read and Vector memory reserve\n";
        std::cout << "7. Memory-Mapped File (Coming Soon)\n";
        std::cout << "8. Binary Format Processing (Coming Soon)\n";
        std::cout << "9. Optimized Multi Thread search\n";
        std::cout << "10. Optimized Multi Thread search\n";
        std::cout << "11. Optimized Multi Thread buffer file read and vector memory reserve\n";
        std::cout << "12. Exit\n";
        std::cout << "=====================================================\n";
        std::cout << "Select processing method: ";

        int choice;
        std::cin >> choice;

        if (choice == 12) {
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
                std::cout << "\nUsing Multi-threaded Buffered File Read Parsing...\n";
                processor = std::make_unique<ProcessorUsingBufferedFileReadThreads>();
                runProcessor(processor);
                break;

            case 6:
                std::cout << "\nUsing Multi-threaded Buffered File Read and Vector reserve Parsing...\n";
                processor = std::make_unique<ProcessorUsingBufferedFileReadVectorReserveThreads>();
                runProcessor(processor);
                break;

            case 7:
                std::cout << "\nMemory-Mapped File Processing is not implemented yet.\n";
                break;

            case 8:
                std::cout << "\nBinary Format Processing is not implemented yet.\n";
                break;

            case 9:
                std::cout << "\nOptimized Multi Thread search...\n";
                processor = std::make_unique<OptimalProcessorUsingThreads>();
                runProcessor(processor);
                break;
            
            case 10:
                std::cout << "\nOptimized Multi Thread using buffer search...\n";
                processor = std::make_unique<OptimalBufferRead>();
                runProcessor(processor);
                break;
            
            case 11:
                std::cout << "\nOptimized Multi Thread using buffer file read and vector memory reserve...\n";
                processor = std::make_unique<OptimalVectorReserve>();
                runProcessor(processor);
                break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}
