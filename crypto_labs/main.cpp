#include "prime_utils.h"
#include "test_lab_1.h"
#include <iostream>

int test_lab_1(){
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
int main() {
    
    int res = test_lab_1();
    return res;
}
