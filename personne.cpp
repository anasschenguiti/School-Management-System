// personne.cpp
#include "personne.h"

void Personne::getInformations() const {
    std::cout << "ID: " << id << "\nNom: " << nom << "\nPrenom: " << prenom
              << "\nEmail: " << email << std::endl;
}

void Personne::modifierInformations(const std::string& newNom, const std::string& newPrenom,
                                  const std::string& newEmail) {
    nom = newNom;
    prenom = newPrenom;
    email = newEmail;
}
