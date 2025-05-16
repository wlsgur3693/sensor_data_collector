#ifndef XOR_ENCODER_H
#define XOR_ENCODER_H

#include <vector>
#include <string>

class XOREncoder {
public:
    XOREncoder();
    ~XOREncoder();

    std::string encode(const std::string& rawData);

private:
    std::vector<uint8_t> m_mask;
    std::vector<uint8_t> generateHashWithMask(const std::vector<uint8_t>& imageData, std::vector<uint8_t> mask);
};

#endif // XOR_ENCODER_H
