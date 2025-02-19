#include "SequentialProcessor/CrashDataProcessor.h"
#include <memory>

int main() {

    std::cout<<"-----------------Approach 1-Single Thread- using ifstream---------------"<<std::endl;
    auto processor = std::make_unique<CrashDataProcessor>();
    processor->loadData("../motor_vehicle_collisions.csv");

    std::cout << "Data load duration: " << processor->getDataLoadDuration().count() << " seconds" << std::endl;

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