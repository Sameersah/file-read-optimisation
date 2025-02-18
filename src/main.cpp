#include "CrashDataProcessor.h"
#include <memory>

int main() {

    std::cout<<"-----------------Approach 1-Single Thread- using ifstream---------------"<<std::endl;
    auto processor = std::make_unique<CrashDataProcessor>();
    processor->loadData("../motor_vehicle_collisions.csv");

    std::cout << "\nCrash Statistics:\n";
    processor->displayStatistics();

    /*
    std::cout << "\nSearching for crashes on 2023-01-01:\n";
    auto results = processor->searchByDate("2023-01-01");
    for (const auto& record : results) {
        std::cout << "Crash on " << record.crash_date << " at " << record.crash_time << " with " << record.persons_injured << " injuries." << std::endl;
    }
    */

    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 2-Buffered File Reading ---------------"<<std::endl;
 //gdfgdgdfghfgf
    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 3-Multi-threaded Parsing ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 4-Memory-Mapped File ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 5- Using Binary Format Instead of CSV ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;






    return 0;
}