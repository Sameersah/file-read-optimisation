#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>


using namespace std;

// Data structure to store crash records
struct CrashRecord {
    string crash_date;
    string crash_time;
    string borough;
    string zip_code;
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
    string contributing_factor_1;
    string contributing_factor_2;
    string vehicle_type_1;
    string vehicle_type_2;
};

// Interface for crash data processing
class ICrashDataProcessor {
public:
    virtual void loadData(const string& filename) = 0;
    virtual vector<CrashRecord> searchByDate(const string& date) const = 0;
    virtual vector<CrashRecord> searchByLocation(float lat, float lon, float radius) const = 0;
    virtual void displayStatistics() const = 0;
    virtual ~ICrashDataProcessor() = default;
};

// Concrete implementation of Crash Data Processor
class CrashDataProcessor final : public ICrashDataProcessor {
private:
    vector<CrashRecord> records;
    unordered_map<string, vector<CrashRecord>> dateIndex;

public:
    void loadData(const string& filename) override {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return;
        }
        string line;
        getline(file, line); // Skip header

        while (getline(file, line)) {
            stringstream ss(line);
            CrashRecord record;
            string temp;

            getline(ss, record.crash_date, ',');
            getline(ss, record.crash_time, ',');
            getline(ss, record.borough, ',');
            getline(ss, record.zip_code, ',');
            getline(ss, temp, ',');
            try { record.latitude = temp.empty() ? 0.0f : stof(temp); }
            catch (const exception&) { record.latitude = 0.0f; }

            getline(ss, temp, ',');
            try { record.longitude = temp.empty() ? 0.0f : stof(temp); }
            catch (const exception&) { record.longitude = 0.0f; }

            getline(ss, temp, ',');
            try { record.persons_injured = temp.empty() ? 0 : stoi(temp); }
            catch (const exception&) { record.persons_injured = 0; }

            getline(ss, temp, ',');
            try { record.persons_killed = temp.empty() ? 0 : stoi(temp); }
            catch (const exception&) { record.persons_killed = 0; }


            getline(ss, temp, ',');
            try {record.pedestrians_injured = temp.empty() ? 0 : stoi(temp);}
            catch (const exception&) { record.pedestrians_injured = 0; }

            getline(ss, temp, ',');
            try {record.pedestrians_killed = temp.empty() ? 0 : stoi(temp);}
            catch (const exception&) { record.pedestrians_killed = 0; }

            getline(ss, temp, ',');
            try {record.cyclists_injured = temp.empty() ? 0 : stoi(temp);}
            catch (const exception&) { record.cyclists_injured = 0; }

            getline(ss, temp, ',');
            try {record.cyclists_killed = temp.empty() ? 0 : stoi(temp);}
            catch (const exception&) { record.cyclists_killed = 0; }

            getline(ss, temp, ',');
            try {record.motorists_injured = temp.empty() ? 0 : stoi(temp);}
            catch (const exception&) { record.motorists_injured = 0; }

            getline(ss, temp, ',');
            try {record.motorists_killed = temp.empty() ? 0 : stoi(temp);}
            catch (const exception&) { record.motorists_killed = 0; }


            getline(ss, record.contributing_factor_1, ',');
            getline(ss, record.contributing_factor_2, ',');
            getline(ss, record.vehicle_type_1, ',');
            getline(ss, record.vehicle_type_2, ',');

            records.push_back(record);
            dateIndex[record.crash_date].push_back(record);
        }
        file.close();
        cout << "Data successfully loaded. Total records: " << records.size() << endl;
    }

    vector<CrashRecord> searchByDate(const string& date) const override {
        auto it = dateIndex.find(date);
        return (it != dateIndex.end()) ? it->second : vector<CrashRecord>{};
    }

    vector<CrashRecord> searchByLocation(float lat, float lon, float radius) const override {
        vector<CrashRecord> result;
        for (const auto& record : records) {
            float dist = sqrt(pow(record.latitude - lat, 2) + pow(record.longitude - lon, 2));
            if (dist <= radius) {
                result.push_back(record);
            }
        }
        return result;
    }

    void displayStatistics() const override {
        int total_crashes = records.size();
        int total_injuries = 0;
        int total_killed = 0;

        for (const auto& record : records) {
            total_injuries += record.persons_injured;
            total_killed += record.persons_killed;
        }

        cout << "Total Crashes: " << total_crashes << endl;
        cout << "Total Injuries: " << total_injuries << endl;
        cout << "Total Fatalities: " << total_killed << endl;
    }
};

// Factory Pattern for creating CrashDataProcessor instances
class CrashDataProcessorFactory {
public:
    static unique_ptr<ICrashDataProcessor> createProcessor() {
        return make_unique<CrashDataProcessor>();
    }
};

int main() {
    auto processor = CrashDataProcessorFactory::createProcessor();
    processor->loadData("../motor_vehicle_collisions.csv");

    cout << "\nCrash Statistics:\n";
    processor->displayStatistics();

    cout << "\nSearching for crashes on 2023-01-01:\n";
    auto results = processor->searchByDate("2023-01-01");
    for (const auto& record : results) {
        cout << "Crash on " << record.crash_date << " at " << record.crash_time << " with " << record.persons_injured << " injuries." << endl;
    }

    cout << "\nSearching for crashes near location (40.7128, -74.0060) within radius 0.01:\n";
    auto locResults = processor->searchByLocation(40.7128, -74.0060, 0.01);
    for (const auto& record : locResults) {
        cout << "Crash at " << record.latitude << ", " << record.longitude << " with " << record.persons_injured << " injuries." << endl;
    }

    return 0;
}
