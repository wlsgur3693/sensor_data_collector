/* 컴파일
g++ -o keygen sign/KeyGenerator.cpp -lssl -lcrypto
./keygen
*/
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>

void generateRSAKey(int bits, const std::string& privateKeyPath, const std::string& publicKeyPath) {
    RSA* rsa = RSA_new();
    BIGNUM* bn = BN_new();
    BN_set_word(bn, RSA_F4);

    if (!RSA_generate_key_ex(rsa, bits, bn, nullptr)) {
        std::cerr << "Error generating RSA key." << std::endl;
        return;
    }

    BIO* pri = BIO_new_file(privateKeyPath.c_str(), "w+");
    if (!PEM_write_bio_RSAPrivateKey(pri, rsa, nullptr, nullptr, 0, nullptr, nullptr)) {
        std::cerr << "Error writing private key to file." << std::endl;
        return;
    }

    BIO* pub = BIO_new_file(publicKeyPath.c_str(), "w+");
    if (!PEM_write_bio_RSAPublicKey(pub, rsa)) {
        std::cerr << "Error writing public key to file." << std::endl;
        return;
    }

    BIO_free_all(pri);
    BIO_free_all(pub);
    RSA_free(rsa);
    BN_free(bn);

    std::cout << "RSA Key Pair Generated:" << std::endl;
    std::cout << "Private Key: " << privateKeyPath << std::endl;
    std::cout << "Public Key: " << publicKeyPath << std::endl;
}

int main() {
    system("mkdir -p keys");

    std::string privateKeyPath = "./keys/private_key.pem";
    std::string publicKeyPath = "./keys/public_key.pem";

    generateRSAKey(2048, privateKeyPath, publicKeyPath);

    return 0;
}
