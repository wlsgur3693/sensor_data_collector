#include "SensorDataCollector.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>

SensorDataCollector::SensorDataCollector()
    : m_temperature(0.0), m_humidity(0.0), 
      m_accelX(0.0), m_accelY(0.0), m_accelZ(0.0),
      m_ultrasonicFile("./sensor_data_collector/ultrasonic.csv"),
      m_dhtFile("./sensor_data_collector/dht22.csv"),
      m_accelFile("./sensor_data_collector/accel.csv"),
      m_running(false) {
}

SensorDataCollector::~SensorDataCollector() {
}

bool SensorDataCollector::loadData() {
    bool result = true;
    result &= loadUltrasonicData();
    result &= loadDHTData();
    result &= loadAccelData();
    return result;
}

bool SensorDataCollector::loadUltrasonicData() {
    std::string line;
    if (std::getline(m_ultrasonicFile, line)) {
        std::stringstream ss(line);
        std::string value;

        m_ultrasonicData.clear();
        m_ultrasonicData.reserve(320);
        int count = 0;
        while (std::getline(ss, value, ',')) {
            m_ultrasonicData.push_back(std::stod(value));
            count++;
        }

        if (count != 320) {
            std::cerr << "Warning: Expected 320 values, but got " << count << std::endl;
        }
        return true;
    }
    return false;
}

bool SensorDataCollector::loadDHTData() {
    std::string line;
    if (std::getline(m_dhtFile, line)) {
        std::stringstream ss(line);
        std::string value;

        if (std::getline(ss, value, ',')) {
            m_temperature = std::stod(value);
        }
        if (std::getline(ss, value, ',')) {
            m_humidity = std::stod(value);
        }
        return true;
    }
    return false;
}

bool SensorDataCollector::loadAccelData() {
    std::string line;
    if (std::getline(m_accelFile, line)) {
        std::stringstream ss(line);
        std::string value;

        if (std::getline(ss, value, ',')) {
            m_accelX = std::stod(value);
        }
        if (std::getline(ss, value, ',')) {
            m_accelY = std::stod(value);
        }
        if (std::getline(ss, value, ',')) {
            m_accelZ = std::stod(value);
        }
        return true;
    }
    return false;
}

Json::Value SensorDataCollector::toJson() const {
    Json::Value j;

    // 1️⃣ Ultrasonic 데이터: 소수점 11자리 고정
    for (const auto& value : m_ultrasonicData) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(11) << value;
        j["ultrasonic"].append(oss.str());
    }

    // 2️⃣ DHT 데이터: 소수점 2자리 고정
    std::ostringstream temp, hum;
    temp << std::fixed << std::setprecision(2) << m_temperature;
    hum << std::fixed << std::setprecision(2) << m_humidity;
    j["dht"]["temperature"] = temp.str();
    j["dht"]["humidity"] = hum.str();

    // 3️⃣ Accel 데이터: 소수점 3자리 고정
    std::ostringstream ax, ay, az;
    ax << std::fixed << std::setprecision(3) << m_accelX;
    ay << std::fixed << std::setprecision(3) << m_accelY;
    az << std::fixed << std::setprecision(3) << m_accelZ;

    j["accel"]["x"] = ax.str();
    j["accel"]["y"] = ay.str();
    j["accel"]["z"] = az.str();

    return j;
}

std::string SensorDataCollector::serialize() const {
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "";
    return Json::writeString(writer, toJson());
}

void SensorDataCollector::start_reading_thread(DataQueue& queue, int interval) {
    if (m_running) {
        std::cerr << "Thread is already running." << std::endl;
        return;
    }
    m_running = true;
    m_readThread = std::thread(&SensorDataCollector::data_reading_task, this, std::ref(queue), interval);
}

void SensorDataCollector::data_reading_task(DataQueue& queue, int interval) {
    pthread_setname_np(pthread_self(), "SensorDataReader");

    while (m_running) {
        if (loadData()) {
            std::string rawData = serialize();
            DataPacket packet{ rawData, "", "" };
            packet.has_data = true;
            queue.push(packet);
            std::cout << "[SensorDataCollector] Data pushed to queue" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
}
