// main.cpp
#include "auth.h"
#include "personne.h"
#include "menu.h"
#include <iostream>

int main() {
    try {
        DonneesTest::chargerDonnees(); // Charger les données au démarrage

        while (true) {
            std::string choice;
            std::cout << "\n=== Authentication System ===\n"
                     << "1. Sign In\n"
                     << "2. Exit\n"
                     << "Choice: ";
            std::getline(std::cin, choice);

            if (choice == "1") {
                std::string email, password;
                std::cout << "\n=== Sign In ===\n"
                         << "Email: ";
                std::getline(std::cin, email);
                std::cout << "Password: ";
                std::getline(std::cin, password);

                try {
                    auto result = Auth::signIn(email, password);
                    bool success = result.first;
                    std::string role = result.second;
                    if (success) {
                        std::cout << "\nConnexion reussie!\n"
                                  << "Role: " << role << "\n";
                        Menu::afficherMenuPrincipal(role, email);
                    } else {
                        std::cout << "\nErreur: Invalid credentials.\n";
                    }
                } catch (const AuthException& e) {
                    std::cout << "\nErreur: " << e.what() << "\n";
                }
            }
            else if (choice == "2") {
                std::cout << "\n \nGoodbye!\n \n";
                break;
            }
            else {
                std::cout << "\nInvalid choice. Please try again.\n";
            }
        }

        DonneesTest::sauvegarderDonnees(); // Sauvegarder ą la fin
        return 0;

    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
