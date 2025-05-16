#ifndef SIGNER_H
#define SIGNER_H

#include <string>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <fstream>
#include <iostream>


class Signer {
public:
    Signer(const std::string& privateKeyPath);
    ~Signer();

    std::string signData(const std::string& data);

private:
    std::string m_privateKeyPath;
    RSA* m_rsa; // ✅ RSA 키를 멤버로 유지
};

#endif // SIGNER_H
