#ifndef RSA_H
#define RSA_H

#include <string>
#include <utility>

class RSA {
public:
    RSA(int bit_length);

    std::pair<unsigned long long, unsigned long long> get_public_key() const;
    std::pair<unsigned long long, unsigned long long> get_private_key() const;

    std::string encrypt(const std::string& message, const std::pair<unsigned long long, unsigned long long>& public_key);
    std::string decrypt(const std::string& cipher_text);

    std::string sign(const std::string& message);
    bool verify(const std::string& message, const std::string& signature, const std::pair<unsigned long long, unsigned long long>& public_key);

private:
    unsigned long long generate_prime(int bit_length);
    unsigned long long compute_carmichael(unsigned long long p, unsigned long long q);
    unsigned long long mod_inverse(unsigned long long a, unsigned long long m);
    unsigned long long hash_message(const std::string& message);
    unsigned long long crt_decrypt(unsigned long long cipher_text);

    unsigned long long p, q, n, e, d;
    unsigned long long dp, dq, qinv;
};

#endif // RSA_H
