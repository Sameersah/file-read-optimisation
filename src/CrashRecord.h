//
// Created by Sameer on 18/02/25.
//

#ifndef CRASHRECORD_H
#define CRASHRECORD_H
#include <string>

struct CrashRecord {
    std::string crash_date;
    std::string crash_time;
    std::string borough;
    std::string zip_code;
    float latitude;
    float longitude;
    int persons_injured;
    int persons_killed;
    int pedestrians_injured;
    int pedestrians_killed;
    int cyclists_injured;
    int cyclists_killed;
    int motorists_injured;
    int motorists_killed;
    std::string contributing_factor_1;
    std::string contributing_factor_2;
    std::string vehicle_type_1;
    std::string vehicle_type_2;
};
#endif //CRASHRECORD_H
