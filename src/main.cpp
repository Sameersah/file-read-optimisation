#include "SequentialProcessor/CrashDataProcessor.h"
#include <memory>

int main() {

    std::cout<<"-----------------Approach 1-Single Thread- using ifstream---------------"<<std::endl;
    auto processor = std::make_unique<CrashDataProcessor>();
    std::cout << "Loading data..." << std::endl;
    processor->loadData("../motor_vehicle_collisions.csv");
    std::cout << "Data load duration: " << processor->getDataLoadDuration().count() << " seconds" << std::endl;
    std::vector<CrashRecord> crashes = processor->getCrashesInDateRange("01/01/2021","01/01/2022");
    std::cout << "Number of crashes in date range: " << crashes.size() << std::endl;
    std::cout << "Date range searching duration: " << processor->getDateRangeSearchingDuration().count() << " seconds" << std::endl;
    std::cout<<"------------------------------------------------------------------------"<<std::endl;

    std::cout<<"-----------------Approach 2-Buffered File Reading ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 3-Multi-threaded Parsing ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 4-Memory-Mapped File ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;

    std::cout<<"-----------------Approach 5- Using Binary Format Instead of CSV ---------------"<<std::endl;

    std::cout<<"-----------------"<<std::endl;






    return 0;
}