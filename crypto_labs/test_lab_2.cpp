#include "prime_utils.h"
#include <iostream>
#include <chrono>

#include "test_lab_2.h"
#include "rsa.h"

void simulate_message_exchange(int bit_length) {
    RSA alice(bit_length);
    RSA bob(bit_length);

    auto alice_public_key = alice.get_public_key();
    auto bob_public_key = bob.get_public_key();

    std::string message = "Hello Bob!";
    std::cout << "Original message: " << message << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::string encrypted_message = alice.encrypt(message, bob_public_key);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> encryption_time = end - start;
    std::cout << "Encrypted message: " << encrypted_message << std::endl;
    std::cout << "Encryption time: " << encryption_time.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::string decrypted_message = bob.decrypt(encrypted_message);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decryption_time = end - start;
    std::cout << "Decrypted message: " << decrypted_message << std::endl;
    std::cout << "Decryption time: " << decryption_time.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::string signature = alice.sign(message);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> signing_time = end - start;
    std::cout << "Signature: " << signature << std::endl;
    std::cout << "Signing time: " << signing_time.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    bool is_verified = bob.verify(message, signature, alice_public_key);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> verification_time = end - start;
    std::cout << "Verification: " << (is_verified ? "success" : "failure") << std::endl;
    std::cout << "Verification time: " << verification_time.count() << " seconds" << std::endl;
}
