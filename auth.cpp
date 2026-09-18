// auth.cpp
#include "auth.h"
#include <sstream>
#include <fstream>


const std::string Auth::USERS_FILE = "users.txt";

void Auth::createUsersFileIfNotExists() {
    std::ifstream testFile(USERS_FILE.c_str());
    if (!testFile) {
        std::ofstream file(USERS_FILE.c_str());
        if (!file.is_open()) {
            throw FileException("Cannot create users file");
        }
        file.close();
    }
    testFile.close();
}

std::vector<Auth::UserCredentials> Auth::loadUsers() {
    createUsersFileIfNotExists();
    std::vector<UserCredentials> users;
    std::ifstream file(USERS_FILE.c_str());

    if (!file.is_open()) {
        throw FileException("Cannot open users file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string email, password, role;

        if (std::getline(ss, email, ',') &&
            std::getline(ss, password, ',') &&
            std::getline(ss, role)) {
            users.push_back({email, password, role});
        }
    }
    return users;
}

bool Auth::isValidEmail(const std::string& email) {
    return !email.empty() && email.find('@') != std::string::npos;
}

std::pair<bool, std::string> Auth::signIn(const std::string& email, const std::string& password) {
    try {
        if (!isValidEmail(email)) {
            throw AuthException("Invalid email format");
        }
        if (password.empty()) {
            throw AuthException("Password cannot be empty");
        }

        auto users = loadUsers();

        for (const auto& user : users) {
            if (user.email == email && user.password == password) {
                return {true, user.role};
            }
        }
        throw AuthException("Invalid credentials");
    } catch (const std::exception& e) {
        throw AuthException(std::string("Sign in failed: ") + e.what());
    }
}
