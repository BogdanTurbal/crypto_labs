#include "prime_utils.h"

void run_prepared_scenario() {
    unsigned long long base = 2;
    unsigned long long exponent = 10;
    unsigned long long modulus = 1000;
    std::cout << "Modular Exponentiation Result: " << modular_exponentiation(base, exponent, modulus) << std::endl;

    unsigned long long n = 61;
    int k = 5;
    if (miller_rabin_test(n, k))
        std::cout << n << " is prime." << std::endl;
    else
        std::cout << n << " is not prime." << std::endl;

    if (baillie_psw_test(n))
        std::cout << n << " is prime." << std::endl;
    else
        std::cout << n << " is not prime." << std::endl;

    std::cout << "Base2: " << to_base2(n) << std::endl;
    std::cout << "Base10: " << to_base10(n) << std::endl;
    std::cout << "Base64: " << to_base64(n) << std::endl;
    std::cout << "Byte Array: " << to_byte_array(n) << std::endl;

    int bits = 6;
    std::vector<unsigned long long> primes = find_primes_with_bit_length(bits);
    std::cout << "Primes with " << bits << " bits:" << std::endl;
    for (unsigned long long prime : primes) {
        std::cout << prime << std::endl;
    }
}

void modular_exponentiation_test() {
    unsigned long long base, exponent, modulus;
    std::cout << "Enter base: ";
    std::cin >> base;
    std::cout << "Enter exponent: ";
    std::cin >> exponent;
    std::cout << "Enter modulus: ";
    std::cin >> modulus;
    std::cout << "Result: " << modular_exponentiation(base, exponent, modulus) << std::endl;
}

void miller_rabin_test_choice() {
    unsigned long long n;
    int k;
    std::cout << "Enter number to test: ";
    std::cin >> n;
    std::cout << "Enter number of iterations: ";
    std::cin >> k;
    if (miller_rabin_test(n, k))
        std::cout << n << " is prime." << std::endl;
    else
        std::cout << n << " is not prime." << std::endl;
}

void baillie_psw_test_choice() {
    unsigned long long n;
    std::cout << "Enter number to test: ";
    std::cin >> n;
    if (baillie_psw_test(n))
        std::cout << n << " is prime." << std::endl;
    else
        std::cout << n << " is not prime." << std::endl;
}

void format_conversions_test() {
    unsigned long long n;
    std::cout << "Enter number to convert: ";
    std::cin >> n;
    std::cout << "Base2: " << to_base2(n) << std::endl;
    std::cout << "Base10: " << to_base10(n) << std::endl;
    std::cout << "Base64: " << to_base64(n) << std::endl;
    std::cout << "Byte Array: " << to_byte_array(n) << std::endl;
}

void find_primes_test() {
    int bits;
    std::cout << "Enter number of bits: ";
    std::cin >> bits;
    std::vector<unsigned long long> primes = find_primes_with_bit_length(bits);
    std::cout << "Primes with " << bits << " bits:" << std::endl;
    for (unsigned long long prime : primes) {
        std::cout << prime << std::endl;
    }
}

int main() {
    while (true) {
        std::cout << "Choose an option:\n";
        std::cout << "1. Run prepared scenario\n";
        std::cout << "2. Modular Exponentiation Test\n";
        std::cout << "3. Miller-Rabin Test\n";
        std::cout << "4. Baillie-PSW Test\n";
        std::cout << "5. Format Conversions Test\n";
        std::cout << "6. Find Primes Test\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                run_prepared_scenario();
                break;
            case 2:
                modular_exponentiation_test();
                break;
            case 3:
                miller_rabin_test_choice();
                break;
            case 4:
                baillie_psw_test_choice();
                break;
            case 5:
                format_conversions_test();
                break;
            case 6:
                find_primes_test();
                break;
            case 7:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }

        std::cout << "\nDo you want to continue? (y/n): ";
        char cont;
        std::cin >> cont;
        if (cont != 'y' && cont != 'Y') {
            break;
        }
    }

    return 0;
}
