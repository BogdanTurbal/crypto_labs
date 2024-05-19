#include "rsa.h"
#include "prime_utils.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>

RSA::RSA(int bit_length) {
    p = generate_prime(bit_length / 2);
    q = generate_prime(bit_length / 2);
    n = p * q;
    unsigned long long carmichael = compute_carmichael(p, q);

    // Choose e such that 1 < e < carmichael and gcd(e, carmichael) = 1
    std::mt19937_64 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long long> dis(2, carmichael - 1);
    do {
        e = dis(gen);
    } while (std::gcd(e, carmichael) != 1);

    d = mod_inverse(e, carmichael);

    // Precompute CRT parameters
    dp = d % (p - 1);
    dq = d % (q - 1);
    qinv = mod_inverse(q, p);
}

std::pair<unsigned long long, unsigned long long> RSA::get_public_key() const {
    return {e, n};
}

std::pair<unsigned long long, unsigned long long> RSA::get_private_key() const {
    return {d, n};
}

unsigned long long RSA::generate_prime(int bit_length) {
    unsigned long long prime;
    do {
        prime = find_primes_with_bit_length(bit_length)[0];
    } while (!baillie_psw_test(prime));
    return prime;
}

unsigned long long RSA::compute_carmichael(unsigned long long p, unsigned long long q) {
    return std::lcm(p - 1, q - 1);
}

unsigned long long RSA::mod_inverse(unsigned long long a, unsigned long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

unsigned long long RSA::hash_message(const std::string& message) {
    std::hash<std::string> hash_fn;
    return hash_fn(message);
}

std::string RSA::encrypt(const std::string& message, const std::pair<unsigned long long, unsigned long long>& public_key) {
    unsigned long long e = public_key.first;
    unsigned long long n = public_key.second;
    std::ostringstream oss;
    for (char c : message) {
        unsigned long long m = static_cast<unsigned long long>(c);
        unsigned long long cipher_text = modular_exponentiation(m, e, n);
        oss << std::hex << std::setw(16) << std::setfill('0') << cipher_text;
    }
    return oss.str();
}

std::string RSA::decrypt(const std::string& cipher_text) {
    std::string message;
    for (size_t i = 0; i < cipher_text.size(); i += 16) {
        std::string hex_str = cipher_text.substr(i, 16);
        unsigned long long cipher = std::stoull(hex_str, nullptr, 16);
        unsigned long long plain = crt_decrypt(cipher);
        message.push_back(static_cast<char>(plain));
    }
    return message;
}

unsigned long long RSA::crt_decrypt(unsigned long long cipher_text) {
    unsigned long long m1 = modular_exponentiation(cipher_text, dp, p);
    unsigned long long m2 = modular_exponentiation(cipher_text, dq, q);
    long long h = (qinv * (m1 - m2)) % p;
    if (h < 0) h += p;
    return m2 + h * q;
}

std::string RSA::sign(const std::string& message) {
    unsigned long long hash = hash_message(message);
    unsigned long long signature = modular_exponentiation(hash, d, n);
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << signature;
    return oss.str();
}

bool RSA::verify(const std::string& message, const std::string& signature, const std::pair<unsigned long long, unsigned long long>& public_key) {
    unsigned long long e = public_key.first;
    unsigned long long n = public_key.second;
    unsigned long long hash = hash_message(message);
    unsigned long long sig = std::stoull(signature, nullptr, 16);
    unsigned long long hash_from_sig = modular_exponentiation(sig, e, n);
    return hash == hash_from_sig;
}
