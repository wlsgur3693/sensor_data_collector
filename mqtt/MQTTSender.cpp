#include "MQTTSender.h"
#include <iostream>
#include <sstream>
#include <mqtt/async_client.h>

MQTTSender::MQTTSender(const std::string& brokerAddress, const std::string& topic)
    : m_brokerAddress(brokerAddress), m_topic(topic), m_client(brokerAddress, ""), m_running(false) {
    m_client.connect()->wait();
}

MQTTSender::~MQTTSender() {

}

void MQTTSender::start_sending_thread(DataQueue& dataQueue) {
    if (!m_running) {
        m_running = true;
        m_sendThread = std::thread(&MQTTSender::sending_task, this, std::ref(dataQueue));
    }
}



void MQTTSender::sending_task(DataQueue& dataQueue) {
    pthread_setname_np(pthread_self(), "MQTT Send Thread");

    while (m_running) {
        if (!dataQueue.isEmpty()) {
            DataPacket& packet = dataQueue.front();
            if (packet.is_signed) {
                send_to_mqtt(packet, dataQueue);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void MQTTSender::send_to_mqtt(const DataPacket& packet, DataQueue& dataQueue) {
    std::cout << "[T3] MQTT 전송 시작..." << std::endl;

    std::stringstream jsonData;
    jsonData << "{"
             << "\"raw_data\": \"" << packet.raw_data << "\", "
             << "\"encoded\": \"" << packet.encoded << "\", "
             << "\"sign_data\": \"" << packet.sign_data << "\""
             << "}";

    mqtt::message_ptr pubmsg = mqtt::make_message(m_topic, jsonData.str());
    pubmsg->set_qos(1);

    try {
        m_client.publish(pubmsg)->wait();
        std::cout << "[T3] MQTT 전송 완료: " << m_topic << std::endl;

        dataQueue.pop();
    } catch (const mqtt::exception& exc) {
        std::cerr << "[T3] MQTT 전송 실패: " << exc.what() << std::endl;
    }
}
