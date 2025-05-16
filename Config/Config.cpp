#include "Config.h"
#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>

Config_cls::Config_cls() {
    Read_Logger_cfg();
}

Config_cls::~Config_cls() {}

void Config_cls::Read_Logger_cfg() {
    std::ifstream configFile("./config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config.json" << std::endl;
        return;
    }

    Json::Value config;
    configFile >> config;

    m_serverIp = config["Logger"]["server_ip"].asString();
    m_serverPort = config["Logger"]["server_port"].asInt();
    m_mqttTopic = config["Logger"]["mqtt_topic"].asString();
    m_privateKeyPath = config["Logger"]["private_key_path"].asString();
    m_publicKeyPath = config["Logger"]["public_key_path"].asString();
    m_readInterval = config["Logger"]["read_interval"].asInt();

    configFile.close();
}

std::string Config_cls::getServerIp() const {
    return m_serverIp;
}

int Config_cls::getServerPort() const {
    return m_serverPort;
}

std::string Config_cls::getMqttTopic() const {
    return m_mqttTopic;
}

std::string Config_cls::getPrivateKeyPath() const {
    return m_privateKeyPath;
}

std::string Config_cls::getPublicKeyPath() const {
    return m_publicKeyPath;
}

int Config_cls::getReadInterval() const {
    return m_readInterval;
}
