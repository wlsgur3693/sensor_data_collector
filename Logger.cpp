#include "sensor_data_collector/SensorDataCollector.h"
#include "Config/Config.h"
#include "queue/DataQueue.h"
#include "encoder/XOREncoder.h"
#include "sign/Signer.h"
#include "mqtt/MQTTSender.h"
#include <iostream>
#include <thread>
#include <unistd.h>

void process_loop() {
    while (true) {
        usleep(1000000); // 1초 대기
    }
}

void encoding_signing_task(DataQueue& dataQueue, XOREncoder& encoder, Signer& signer) {
    pthread_setname_np(pthread_self(), "Encoding & Signing Thread");

    while (true) {
        if (!dataQueue.isEmpty()) {
            DataPacket& packet = dataQueue.front();

            if (!packet.is_signed && packet.has_data) {
                std::cout << "[T2] 인코딩 및 서명 시작..." << std::endl;

                packet.encoded = encoder.encode(packet.raw_data);

                packet.sign_data = signer.signData(packet.encoded);

                packet.is_signed = true;

                std::cout << "[T2] 인코딩 및 서명 완료" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main() {
    Config_cls config;
    SensorDataCollector collector;
    DataQueue dataQueue;
    XOREncoder encoder;

    Signer signer(config.getPrivateKeyPath());

    std::string brokerAddress = "tcp://" + config.getServerIp() + ":" + std::to_string(config.getServerPort());
    MQTTSender sender(brokerAddress, config.getMqttTopic());

    int interval = config.getReadInterval();
    std::cout << "Read Interval 설정값: " << interval << " ms" << std::endl;

    //t1
    collector.start_reading_thread(dataQueue, interval);

    //t2
    std::thread t2(encoding_signing_task, std::ref(dataQueue), std::ref(encoder), std::ref(signer));

    //t3
    sender.start_sending_thread(dataQueue);

    process_loop();

    return 0;
}
