#ifndef PRIME_UTILS_H
#define PRIME_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <random>

// Modular Exponentiation
unsigned long long modular_exponentiation(unsigned long long base, unsigned long long exponent, unsigned long long modulus);

// Miller-Rabin Test
bool miller_rabin_test(unsigned long long n, int k);

// Baillie-PSW Test
bool baillie_psw_test(unsigned long long n);

// Format Conversions
std::string to_base2(unsigned long long num);
std::string to_base10(unsigned long long num);
std::string to_base64(unsigned long long num);
std::string to_byte_array(unsigned long long num);

// Prime Finder
std::vector<unsigned long long> find_primes_with_bit_length(int bits);

// Helper functions for Lucas test
int jacobi_symbol(long long a, long long n);
bool lucas_pseudoprime_test(unsigned long long n, long long D, unsigned long long P, unsigned long long Q);

#endif // PRIME_UTILS_H
