#ifndef SENSOR_DATA_COLLECTOR_H
#define SENSOR_DATA_COLLECTOR_H

#include <string>
#include <vector>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <thread>
#include <atomic>
#include "../queue/DataQueue.h"

class SensorDataCollector {
public:
    SensorDataCollector();
    ~SensorDataCollector();

    bool loadData();
    Json::Value toJson() const;
    std::string serialize() const;

    void start_reading_thread(DataQueue& queue, int interval);
    void stop_reading_thread();

private:
    std::vector<double> m_ultrasonicData;
    double m_temperature;
    double m_humidity;
    double m_accelX;
    double m_accelY;
    double m_accelZ;

    std::ifstream m_ultrasonicFile;
    std::ifstream m_dhtFile;
    std::ifstream m_accelFile;

    std::atomic<bool> m_running;       // 스레드 종료 플래그
    std::thread m_readThread;          // 읽기 스레드

    bool loadUltrasonicData();
    bool loadDHTData();
    bool loadAccelData();
    void data_reading_task(DataQueue& queue, int interval);
};

#endif // SENSOR_DATA_COLLECTOR_H
