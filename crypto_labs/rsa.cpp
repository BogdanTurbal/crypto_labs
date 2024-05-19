#include "rsa.h"
#include "prime_utils.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <functional>  // For std::hash

RSA::RSA(int bit_length) {
    std::mt19937_64 gen(std::random_device{}());
    p = generate_prime(bit_length / 2);
    q = generate_prime(bit_length / 2);

    // Ensure p and q are different
    while (q == p) {
        q = generate_prime(bit_length / 2);
    }

    std::cout << "p: " << p << ", q: " << q << std::endl;

    n = p * q;
    unsigned long long carmichael = compute_carmichael(p, q);

    // Choose e such that 1 < e < carmichael and gcd(e, carmichael) = 1
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
    auto primes = find_primes_with_bit_length(bit_length);
    std::mt19937_64 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dis(0, primes.size() - 1);
    unsigned long long prime;
    do {
        prime = primes[dis(gen)];
    } while (!miller_rabin_test(prime, 200));
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

const std::array<unsigned long long, 8> initial_hash_values = {
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
    0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

const std::array<unsigned long long, 80> round_constants = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

unsigned long long right_rotate(unsigned long long value, unsigned int count) {
    return (value >> count) | (value << (64 - count));
}

std::vector<unsigned long long> pad_message(const std::string& message) {
    std::vector<unsigned long long> padded_message(message.begin(), message.end());
    padded_message.push_back(0x80);  // Append '1' bit to message
    while ((padded_message.size() % 128) != 112) {
        padded_message.push_back(0);  // Append '0' bits until message is 896 mod 1024 bits
    }
    unsigned long long message_length = message.size() * 8;
    for (int i = 0; i < 8; ++i) {
        padded_message.push_back(static_cast<unsigned long long>(message_length >> (56 - 8 * i)));
    }
    return padded_message;
}

std::array<unsigned long long, 8> sha256_transform(const std::array<unsigned long long, 8>& hash_values, const std::vector<unsigned long long>& message_block) {
    std::array<unsigned long long, 8> working_vars = hash_values;
    std::array<unsigned long long, 80> w;

    for (size_t t = 0; t < 16; ++t) {
        w[t] = message_block[t];
    }
    for (size_t t = 16; t < 80; ++t) {
        unsigned long long s0 = right_rotate(w[t - 15], 1) ^ right_rotate(w[t - 15], 8) ^ (w[t - 15] >> 7);
        unsigned long long s1 = right_rotate(w[t - 2], 19) ^ right_rotate(w[t - 2], 61) ^ (w[t - 2] >> 6);
        w[t] = w[t - 16] + s0 + w[t - 7] + s1;
    }

    for (size_t t = 0; t < 80; ++t) {
        unsigned long long S1 = right_rotate(working_vars[4], 14) ^ right_rotate(working_vars[4], 18) ^ right_rotate(working_vars[4], 41);
        unsigned long long ch = (working_vars[4] & working_vars[5]) ^ (~working_vars[4] & working_vars[6]);
        unsigned long long temp1 = working_vars[7] + S1 + ch + round_constants[t] + w[t];
        unsigned long long S0 = right_rotate(working_vars[0], 28) ^ right_rotate(working_vars[0], 34) ^ right_rotate(working_vars[0], 39);
        unsigned long long maj = (working_vars[0] & working_vars[1]) ^ (working_vars[0] & working_vars[2]) ^ (working_vars[1] & working_vars[2]);
        unsigned long long temp2 = S0 + maj;

        working_vars[7] = working_vars[6];
        working_vars[6] = working_vars[5];
        working_vars[5] = working_vars[4];
        working_vars[4] = working_vars[3] + temp1;
        working_vars[3] = working_vars[2];
        working_vars[2] = working_vars[1];
        working_vars[1] = working_vars[0];
        working_vars[0] = temp1 + temp2;
    }

    std::array<unsigned long long, 8> new_hash_values;
    for (size_t i = 0; i < 8; ++i) {
        new_hash_values[i] = hash_values[i] + working_vars[i];
    }
    return new_hash_values;
}

unsigned long long RSA::custom_hash(const std::string& message) {
    std::vector<unsigned long long> padded_message = pad_message(message);
    std::array<unsigned long long, 8> hash_values = initial_hash_values;

    for (size_t i = 0; i < padded_message.size(); i += 16) {
        std::vector<unsigned long long> message_block(padded_message.begin() + i, padded_message.begin() + i + 16);
        hash_values = sha256_transform(hash_values, message_block);
    }

    // Return only the first 64 bits of the resulting hash value
    return static_cast<unsigned short>(hash_values[0] & 0xFFFF);
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
        if (plain > 255) throw std::runtime_error("Decryption error: plain text value out of ASCII range");
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
    unsigned long long hash = custom_hash(message);
    std::cout << "Message hash: " << hash << std::endl;
    unsigned long long signature = modular_exponentiation(hash, d, n);
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << signature;
    return oss.str();
}

bool RSA::verify(const std::string& message, const std::string& signature, const std::pair<unsigned long long, unsigned long long>& public_key) {
    unsigned long long e = public_key.first;
    unsigned long long n = public_key.second;
    unsigned long long hash = custom_hash(message);
    unsigned long long sig = std::stoull(signature, nullptr, 16);
    unsigned long long hash_from_sig = modular_exponentiation(sig, e, n);
    std::cout << "Hash from signature: " << hash_from_sig << std::endl;
    std::cout << "Expected hash: " << hash << std::endl;
    return hash == hash_from_sig;
}
