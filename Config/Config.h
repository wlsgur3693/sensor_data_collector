#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config_cls {
public:
    Config_cls();
    ~Config_cls();

    std::string getServerIp() const;
    int getServerPort() const;
    std::string getMqttTopic() const;
    std::string getPrivateKeyPath() const;
    std::string getPublicKeyPath() const;
    int getReadInterval() const;

private:
    void Read_Logger_cfg();

    std::string m_serverIp;
    int m_serverPort;
    std::string m_mqttTopic;
    std::string m_privateKeyPath;
    std::string m_publicKeyPath;
    int m_readInterval;
};

#endif // CONFIG_H
