#include "Signer.h"

Signer::Signer(const std::string& privateKeyPath) {
    FILE* privateKeyFile = fopen(privateKeyPath.c_str(), "r");
    if (!privateKeyFile) {
        std::cerr << "Failed to open private key file." << std::endl;
        m_rsa = nullptr;
        return;
    }

    m_rsa = PEM_read_RSAPrivateKey(privateKeyFile, nullptr, nullptr, nullptr);
    fclose(privateKeyFile);

    if (!m_rsa) {
        std::cerr << "Failed to read private key." << std::endl;
    } else {
        std::cout << "[Signer] Private key successfully loaded." << std::endl;
    }
}

Signer::~Signer() {}

std::string base64_encode(const unsigned char* buffer, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    BIO_push(b64, bio);

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, buffer, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bufferPtr);

    std::string encodedData(bufferPtr->data, bufferPtr->length);

    BIO_free_all(b64);
    return encodedData;
}

std::string Signer::signData(const std::string& data) {
    if (!m_rsa) {
        std::cerr << "Private key not loaded." << std::endl;
        return "";
    }

    int rsaSize = RSA_size(m_rsa);
    unsigned char* signature = new unsigned char[rsaSize];

    int result = RSA_private_encrypt(
        data.size(),
        reinterpret_cast<const unsigned char*>(data.c_str()),
        signature,
        m_rsa,
        RSA_PKCS1_PADDING
    );

    if (result == -1) {
        std::cerr << "Failed to sign data: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
        delete[] signature;
        return "";
    }

    std::string signData = base64_encode(signature, result);

    delete[] signature;

    return signData;
}
