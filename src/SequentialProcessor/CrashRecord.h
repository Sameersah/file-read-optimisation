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
    std::string location;
    std::string on_street_name;
    std::string cross_street_name;
    std::string off_street_name;
    int persons_injured;
    int persons_killed;
    int pedestrians_injured;
    int pedestrians_killed;
    int cyclists_injured;
    int cyclists_killed;
    int motorists_injured;
    int motorists_killed;
    std::string contributing_factor_vehicle_1;
    std::string contributing_factor_vehicle_2;
    std::string contributing_factor_vehicle_3;
    std::string contributing_factor_vehicle_4;
    std::string contributing_factor_vehicle_5;
    long collision_id;
    std::string vehicle_type_1;
    std::string vehicle_type_2;
    std::string vehicle_type_code_1;
    std::string vehicle_type_code_2;
    std::string vehicle_type_code_3;
    std::string vehicle_type_code_4;
    std::string vehicle_type_code_5;
    std::string vehicle_type_code_6;
};
#endif //CRASHRECORD_H
