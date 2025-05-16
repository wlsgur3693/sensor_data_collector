#include "DataQueue.h"
#include <stdexcept>

void DataQueue::push(const DataPacket& data) {
    m_mutex.lock();
    m_queue.push(data);
    m_mutex.unlock();
}

DataPacket& DataQueue::front() {
    m_mutex.lock();
    if (m_queue.empty()) {
        m_mutex.unlock();
        throw std::runtime_error("Queue is empty");
    }
    DataPacket& front = m_queue.front();
    m_mutex.unlock();
    return front;
}

void DataQueue::pop() {
    m_mutex.lock();
    if (!m_queue.empty()) {
        m_queue.pop();
    }
    m_mutex.unlock();
}

bool DataQueue::isEmpty() const {
    m_mutex.lock();
    bool isEmpty = m_queue.empty();
    m_mutex.unlock();
    return isEmpty;
}
