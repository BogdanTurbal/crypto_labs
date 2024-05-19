#include "prime_utils.h"

unsigned long long modular_exponentiation(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
    unsigned long long result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

bool miller_rabin_test(unsigned long long n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    unsigned long long d = n - 1;
    while (d % 2 == 0) d /= 2;

    std::mt19937_64 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long long> dis(2, n - 2);

    for (int i = 0; i < k; ++i) {
        unsigned long long a = dis(gen);
        unsigned long long x = modular_exponentiation(a, d, n);
        if (x == 1 || x == n - 1) continue;

        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;

            if (x == 1) return false;
            if (x == n - 1) break;
        }

        if (x != n - 1) return false;
    }
    return true;
}

int jacobi_symbol(long long a, long long n) {
    if (n <= 0 || n % 2 == 0) return 0;
    int result = 1;
    if (a < 0) {
        a = -a;
        if (n % 4 == 3) result = -result;
    }
    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5) result = -result;
        }
        std::swap(a, n);
        if (a % 4 == 3 && n % 4 == 3) result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

bool lucas_pseudoprime_test(unsigned long long n, long long D, unsigned long long P, unsigned long long Q) {
    unsigned long long k = n + 1;
    unsigned long long d = k;
    unsigned long long s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    unsigned long long U = 1;
    unsigned long long V = P;
    unsigned long long U2m = 1;
    unsigned long long V2m = P;
    unsigned long long Qm = Q;
    unsigned long long qm = Q;

    for (long long bit = (1LL << (63 - __builtin_clzll(d))); bit; bit >>= 1) {
        U2m = (U * V) % n;
        V2m = (V * V + Qm * U * U) % n;
        U = (U2m * (P * U + V)) % n;
        V = (V2m * V) % n;
        Qm = (Qm * Qm) % n;

        if (d & bit) {
            unsigned long long tU = U;
            U = (U2m * V + U * V2m) % n;
            V = (V2m * V + D * U2m * tU) % n;
            Qm = (Qm * Q) % n;
        }
    }

    if (U == 0 || V == 0) return true;
    for (unsigned long long r = 0; r < s - 1; r++) {
        V = (V * V - 2 * Qm) % n;
        if (V == 0) return true;
        Qm = (Qm * Qm) % n;
    }

    return false;
}

bool baillie_psw_test(unsigned long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    if (!miller_rabin_test(n, 1)) return false; // Base-2 Miller-Rabin test

    long long D = 5;
    int sign = 1;
    while (jacobi_symbol(D, n) != -1) {
        D = D + sign * 2;
        sign = -sign;
    }

    return lucas_pseudoprime_test(n, D, 1, (1 - D) / 4);
}

std::string to_base2(unsigned long long num) {
    return std::bitset<64>(num).to_string();
}

std::string to_base10(unsigned long long num) {
    return std::to_string(num);
}

std::string to_base64(unsigned long long num) {
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string base64;
    while (num > 0) {
        base64 = base64_chars[num % 64] + base64;
        num /= 64;
    }
    return base64.empty() ? "0" : base64;
}

std::string to_byte_array(unsigned long long num) {
    std::ostringstream oss;
    for (int i = sizeof(num) - 1; i >= 0; --i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << ((num >> (i * 8)) & 0xFF);
    }
    return oss.str();
}

std::vector<unsigned long long> find_primes_with_bit_length(int bits) {
    std::vector<unsigned long long> primes;
    unsigned long long start = 1ULL << (bits - 1);
    unsigned long long end = (1ULL << bits) - 1;

    for (unsigned long long num = start; num <= end; ++num) {
        if (miller_rabin_test(num, 100)) {
            primes.push_back(num);
        }
    }
    return primes;
}
