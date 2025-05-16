#include "XOREncoder.h"
#include <iostream>
#include <vector>

XOREncoder::XOREncoder() {
    //임의값  - 수정해야함 
    m_mask = {0x55, 0x55, 0x55, 0x55};
}

XOREncoder::~XOREncoder() {}

std::vector<uint8_t> XOREncoder::generateHashWithMask(const std::vector<uint8_t>& imageData, std::vector<uint8_t> mask) {
    size_t maskSize = mask.size();
    size_t imageSize = imageData.size();

    for (size_t i = 0; i < imageSize; i += maskSize) {
        for (size_t j = 0; j < maskSize && (i + j) < imageSize; j++) {
            mask[j] = mask[j] ^ imageData[i + j];
        }
    }

    return mask;
}

std::string XOREncoder::encode(const std::string& rawData) {
    // 문자열을 바이트 배열로 변환
    std::vector<uint8_t> imageData(rawData.begin(), rawData.end());

    // XOR 연산을 수행하여 해시 생성
    std::vector<uint8_t> encodedData = generateHashWithMask(imageData, m_mask);

    // 결과를 16진수 문자열로 변환
    std::string encodedStr;
    for (uint8_t byte : encodedData) {
        char buffer[3];
        snprintf(buffer, sizeof(buffer), "%02X", byte);
        encodedStr += buffer;
    }

    return encodedStr;
}
