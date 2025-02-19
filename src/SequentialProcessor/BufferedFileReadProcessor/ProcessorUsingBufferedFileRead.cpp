#include "../common/CrashRecord.h"
#include "ProcessorUsingBufferedFileRead.h"

void ProcessorUsingBufferedFileRead::loadData(const std::string& filename) {

}

std::vector<CrashRecord> ProcessorUsingBufferedFileRead::getCrashesInDateRange(const std::string& start_date, const std::string& end_date)  {
    return {};
}

std::vector<CrashRecord> ProcessorUsingBufferedFileRead::getCrashesByInjuryCountRange(int min_injuries, int max_injuries)  {
    return {};
}

std::vector<CrashRecord> ProcessorUsingBufferedFileRead::getCrashesByLocationRange(float lat, float lon, float radius)  {
    return {};
}


std::chrono::duration<double> ProcessorUsingBufferedFileRead::getDataLoadDuration() const {
    return data_load_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileRead::getDateRangeSearchingDuration() const {
    return date_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileRead::getInjuryRangeSearchingDuration() const {
    return injury_range_Searching_duration;
}

std::chrono::duration<double> ProcessorUsingBufferedFileRead::getLocationRangeSearchingDuration() const {
    return location_range_Searching_duration;
}
