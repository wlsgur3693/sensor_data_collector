#ifndef MQTT_SENDER_H
#define MQTT_SENDER_H

#include <string>
#include <mqtt/async_client.h>
#include <thread>
#include <atomic>
#include "../queue/DataQueue.h"

class MQTTSender {
public:
    MQTTSender(const std::string& brokerAddress, const std::string& topic);
    ~MQTTSender();

    void start_sending_thread(DataQueue& dataQueue);

private:
    void sending_task(DataQueue& dataQueue);
    void send_to_mqtt(const DataPacket& packet, DataQueue& dataQueue); // ✅ 추가된 메서드

    std::string m_brokerAddress;
    std::string m_topic;
    mqtt::async_client m_client;
    std::thread m_sendThread;
    std::atomic<bool> m_running;
};

#endif // MQTT_SENDER_H
