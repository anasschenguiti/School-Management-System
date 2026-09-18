// auth.h
#ifndef AUTH_H
#define AUTH_H

#include "utils.h"
#include <string>
#include <utility>
#include <vector>
#include <fstream>

class Auth {
private:
    static const std::string USERS_FILE;

    struct UserCredentials {
        std::string email;
        std::string password;
        std::string role;
    };

    static void createUsersFileIfNotExists();
    static std::vector<UserCredentials> loadUsers();
    static bool isValidEmail(const std::string& email);

public:
    static std::pair<bool, std::string> signIn(const std::string& email, const std::string& password);
};

#endif
