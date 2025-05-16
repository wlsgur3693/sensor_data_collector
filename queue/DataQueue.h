#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <string>
#include <queue>
#include <mutex>
#include <iostream>

// ===============================
// 데이터 구조체 정의
// ===============================
struct DataPacket {
    std::string raw_data;
    std::string encoded;
    std::string sign_data;
    bool has_data = false;
    bool is_signed = false;
};

// ===============================
// Thread-safe Queue 정의
// ===============================
class DataQueue {
public:
    void push(const DataPacket& data);
    DataPacket& front();
    void pop();
    bool isEmpty() const;

private:
    std::queue<DataPacket> m_queue;
    mutable std::mutex m_mutex;
};
#endif // DATA_QUEUE_H
