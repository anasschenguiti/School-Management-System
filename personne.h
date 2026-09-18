// personne.h
#ifndef PERSONNE_H
#define PERSONNE_H
#include <string>
#include <iostream>


class Personne {
protected:
    int id;
    std::string nom;
    std::string prenom;
    std::string email;

public:
    Personne(int id, const std::string& nom, const std::string& prenom, const std::string& email)
        : id(id), nom(nom), prenom(prenom), email(email) {}

    virtual ~Personne() = default;

    virtual void getInformations() const;
    virtual void modifierInformations(const std::string& newNom, const std::string& newPrenom,
                                    const std::string& newEmail);
};

#endif
