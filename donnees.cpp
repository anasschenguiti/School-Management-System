// donnees.cpp
#include "donnees.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
std::vector<Etudiant> DonneesTest::etudiants;
std::vector<Cours> DonneesTest::cours;
std::vector<Filiere> DonneesTest::filieres;
std::vector<Groupe> DonneesTest::groupes;


void DonneesTest::sauvegarderTout() {
    sauvegarderEtudiants();
    sauvegarderCours();
    sauvegarderFilieres();
    sauvegarderGroupes();
}

void DonneesTest::chargerDonnees() {
    etudiants.clear();
    cours.clear();
    filieres.clear();
    groupes.clear();

   try {
        chargerEtudiants();
        chargerCours();
        chargerFilieres();
        chargerGroupes();

        for (const auto& etudiant : etudiants) {
            if (std::find_if(filieres.begin(), filieres.end(),
                            [&](const Filiere& f) { return f.nom == etudiant.filiere; })
                == filieres.end()) {
                std::cerr << "Attention: Etudiant " << etudiant.nom << " " << etudiant.prenom
                         << " fait reference à une filiere inexistante: "
                         << etudiant.filiere << std::endl;
            }
        }
        for (const auto& groupe : groupes) {
            for (int etudiantId : groupe.etudiantsIds) {
                if (!getEtudiantParId(etudiantId)) {
                    std::cerr << "Attention: Groupe " << groupe.nom
                             << " fait reference à un etudiant inexistant: "
                             << etudiantId << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des données: " << e.what() << std::endl;
        throw;
    }

    try {

    std::ifstream notesFile("notes.txt");
    if (notesFile.is_open()) {
        std::string line;
        while (std::getline(notesFile, line)) {
            std::stringstream ss(line);
            std::string idStr, matiere, valeurStr, appreciation, date;
            if (std::getline(ss, idStr, ',') &&
                std::getline(ss, matiere, ',') &&
                std::getline(ss, valeurStr, ',') &&
                std::getline(ss, appreciation, ',') &&
                std::getline(ss, date)) {
                int id = std::stoi(idStr);
                float valeur = std::stof(valeurStr);
                Etudiant* etudiant = getEtudiantParId(id);
                if (etudiant) {
                    etudiant->notes.push_back({valeur, appreciation, date, matiere});
                }
            }
        }
    }
     notesFile.close();

    std::ifstream absencesFile("absences.txt");
    if (absencesFile.is_open()) {
        std::string line;
        while (std::getline(absencesFile, line)) {
            std::stringstream ss(line);
            std::string idStr, matiere, date, justifieStr, motif;
            if (std::getline(ss, idStr, ',') &&
                std::getline(ss, matiere, ',') &&
                std::getline(ss, date, ',') &&
                std::getline(ss, justifieStr, ',') &&
                std::getline(ss, motif)) {
                int id = std::stoi(idStr);
                bool justifie = (justifieStr == "1" || justifieStr == "true");
                Etudiant* etudiant = getEtudiantParId(id);
                if (etudiant) {
                    etudiant->absences.push_back({date, justifie, motif, matiere});
                }
            }
        }
    }
    absencesFile.close();
    }
     catch (const std::exception& e) {
        // Nettoyage en cas d'erreur
        etudiants.clear();
        cours.clear();
        filieres.clear();
        groupes.clear();
        throw;
    }
}

void DonneesTest::sauvegarderDonnees() {
    sauvegarderEtudiants();
    sauvegarderCours();
    sauvegarderFilieres();
    sauvegarderGroupes();
}

void DonneesTest::chargerEtudiants() {
    etudiants.clear();
    std::ifstream file("etudiants.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, nom, prenom, email, filiere, groupe, photoPath;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, nom, ',') &&
            std::getline(ss, prenom, ',') &&
            std::getline(ss, email, ',') &&
            std::getline(ss, filiere, ',') &&
            std::getline(ss, groupe, ',') &&
            std::getline(ss, photoPath)) {

            int id = std::stoi(idStr);
            std::vector<Note> notes;
            std::vector<Absence> absences;
            etudiants.push_back({id, nom, prenom, email, filiere, groupe, notes, absences, photoPath});
        }
    }
    file.close();
}

void DonneesTest::sauvegarderEtudiants() {
    std::ofstream file("etudiants.txt", std::ios::trunc);
    if (!file.is_open()) return;

    for (const auto& etudiant : etudiants) {
        file << etudiant.id << ","
             << etudiant.nom << ","
             << etudiant.prenom << ","
             << etudiant.email << ","
             << etudiant.filiere << ","
             << etudiant.groupe << ","
             << etudiant.photoPath << "\n";
    }
    file.close();

    std::ofstream notesFile("notes.txt", std::ios::trunc);
    if (notesFile.is_open()) {
        for (const auto& etudiant : etudiants) {
            for (const auto& note : etudiant.notes) {
                notesFile << etudiant.id << ","
                         << note.matiere << ","
                         << note.valeur << ","
                         << note.appreciation << ","
                         << note.date << "\n";
            }
        }
        notesFile.close();
    }

    std::ofstream absencesFile("absences.txt", std::ios::trunc);
    if (absencesFile.is_open()) {
        for (const auto& etudiant : etudiants) {
            for (const auto& absence : etudiant.absences) {
                absencesFile << etudiant.id << ","
                            << absence.matiere << ","
                            << absence.date << ","
                            << absence.justifie << ","
                            << absence.motif << "\n";
            }
        }
        absencesFile.close();
    }
}

void DonneesTest::chargerCours() {
    cours.clear();
    std::ifstream file("cours.txt");
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir cours.txt" << std::endl; // Debug
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string code, nom, coefficientStr, salle, professeur;

        if (std::getline(ss, code, ',') &&
            std::getline(ss, nom, ',') &&
            std::getline(ss, coefficientStr, ',') &&
            std::getline(ss, salle, ',') &&
            std::getline(ss, professeur)) {
            try {
                int coefficient = std::stoi(coefficientStr);
                cours.push_back({code, nom, coefficient, salle, professeur});

            } catch (const std::exception& e) {
                std::cerr << "Erreur lors du chargement d'un cours: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Format invalide pour la ligne : " << line << std::endl; // Debug
        }
    }
    file.close();
}

void DonneesTest::sauvegarderCours() {
    std::ofstream file("cours.txt");
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir cours.txt pour écriture" << std::endl;
        return;
    }

    for (const auto& c : cours) {
        file << c.code << ","
             << c.nom << ","
             << c.coefficient << ","
             << c.salle << ","
             << c.professeur << "\n";
    }
    file.close();
}

Etudiant* DonneesTest::getEtudiantParId(int id) {
    for (auto& etudiant : etudiants) {
        if (etudiant.id == id) return &etudiant;
    }
    return nullptr;
}

Etudiant* DonneesTest::getEtudiantParEmail(const std::string& email) {
    for (auto& etudiant : etudiants) {
        if (etudiant.email == email) return &etudiant;
    }
    return nullptr;
}

std::vector<Etudiant> DonneesTest::getEtudiantsParFiliere(const std::string& filiere) {
    std::vector<Etudiant> result;
    for (const auto& etudiant : etudiants) {
        if (etudiant.filiere == filiere) {
            result.push_back(etudiant);
        }
    }
    return result;
}

std::vector<Etudiant> DonneesTest::getEtudiantsParGroupe(const std::string& groupe) {
    std::vector<Etudiant> result;
    for (const auto& etudiant : etudiants) {
        if (etudiant.groupe == groupe) {
            result.push_back(etudiant);
        }
    }
    return result;
}

bool DonneesTest::ajouterNote(int etudiantId, const Note& note) {
    if (!validerNote(note)) {
        std::cerr << "Erreur: données de note invalides" << std::endl;
        return false;
    }

    if (note.valeur < 0 || note.valeur > 20) {
        std::cerr << "Erreur: La note doit etre comprise entre 0 et 20" << std::endl;
        return false;
    }

    if (note.date.empty() || note.date.length() != 10) {
        std::cerr << "Erreur: Format de date invalide" << std::endl;
        return false;
    }

    auto* etudiant = getEtudiantParId(etudiantId);
    if (!etudiant) {
        std::cerr << "Erreur: Etudiant non trouve" << std::endl;
        return false;
    }

    etudiant->notes.push_back(note);
    sauvegarderEtudiants();
    return true;
}

int DonneesTest::getNewEtudiantId() {
    if (etudiants.empty()) {
        return 1;
    }
    return etudiants.back().id + 1;
}

bool DonneesTest::modifierNote(int etudiantId, const std::string& matiere, float nouvelleNote) {
    for (auto& etudiant : etudiants) {
        if (etudiant.id == etudiantId) {
            for (auto& note : etudiant.notes) {
                if (note.matiere == matiere) {
                    note.valeur = nouvelleNote;
                    return true;
                }
            }
        }
    }
    return false;
}

bool DonneesTest::ajouterAbsence(int etudiantId, const Absence& absence) {
      if (!validerAbsence(absence)) {
        std::cerr << "Erreur: données d'absence invalides" << std::endl;
        return false;
    }

    for (auto& etudiant : etudiants) {
        if (etudiant.id == etudiantId) {
            etudiant.absences.push_back(absence);
            sauvegarderEtudiants();
            return true;
        }
    }
    return false;
}

void DonneesTest::ajouterEtudiant(const Etudiant& etudiant) {
    etudiants.push_back(etudiant);
    sauvegarderEtudiants();
}

void DonneesTest::supprimerEtudiant(int id) {
    etudiants.erase(
        std::remove_if(etudiants.begin(), etudiants.end(),
                      [id](const Etudiant& e) { return e.id == id; }),
        etudiants.end());
    sauvegarderEtudiants();
}

void DonneesTest::ajouterFiliere(const Filiere& filiere) {
    filieres.push_back(filiere);
     sauvegarderFilieres();
}

void DonneesTest::supprimerFiliere(const std::string& nom) {
    filieres.erase(std::remove_if(filieres.begin(), filieres.end(),
                                 [nom](const Filiere& f) { return f.nom == nom; }),
                   filieres.end());
                   sauvegarderFilieres();
}

void DonneesTest::ajouterGroupe(const Groupe& groupe) {
    groupes.push_back(groupe);
      sauvegarderGroupes();
}

void DonneesTest::supprimerGroupe(const std::string& nom) {
    groupes.erase(std::remove_if(groupes.begin(), groupes.end(),
                                [nom](const Groupe& g) { return g.nom == nom; }),
                  groupes.end());
                   sauvegarderGroupes();
}

void DonneesTest::chargerFilieres() {
    std::ifstream file("filieres.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nom, departement, coursCode;
        std::vector<std::string> coursList;
        if (std::getline(ss, nom, ',') &&
            std::getline(ss, departement, ',')) {
            while (std::getline(ss, coursCode, ',')) {
                coursList.push_back(coursCode);
            }
            filieres.push_back({nom, departement, coursList});
        }
    }
    file.close();
}

void DonneesTest::sauvegarderFilieres() {
    std::ofstream file("filieres.txt", std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir filieres.txt pour écriture" << std::endl;
        return;
    }

    for (const auto& filiere : filieres) {
        file << filiere.nom << "," << filiere.departement;
        for (const auto& coursCode : filiere.cours) {
            file << "," << coursCode;
        }
        file << "\n";
    }
    file.close();
}

void DonneesTest::chargerGroupes() {
    std::ifstream file("groupes.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nom, filiere, etudiantIdStr;
        std::vector<int> etudiantsIds;
        if (std::getline(ss, nom, ',') &&
            std::getline(ss, filiere, ',')) {
            while (std::getline(ss, etudiantIdStr, ',')) {
                int etudiantId = std::stoi(etudiantIdStr);
                etudiantsIds.push_back(etudiantId);
            }
            groupes.push_back({nom, filiere, etudiantsIds});
        }
    }
    file.close();
}

void DonneesTest::sauvegarderGroupes() {
    std::ofstream file("groupes.txt", std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir groupes.txt pour écriture" << std::endl;
        return;
    }

    for (const auto& groupe : groupes) {
        file << groupe.nom << "," << groupe.filiere;
        for (const auto& etudiantId : groupe.etudiantsIds) {
            file << "," << etudiantId;
        }
        file << "\n";
    }
    file.close();
}

void DonneesTest::ajouterCours(const Cours& nouveauCours) {
    cours.push_back(nouveauCours);
    sauvegarderCours();
}

void DonneesTest::supprimerCours(const std::string& code) {
    cours.erase(
        std::remove_if(cours.begin(), cours.end(),
                      [code](const Cours& c) { return c.code == code; }),
        cours.end());
    sauvegarderCours();
}

bool DonneesTest::validerNote(const Note& note) {
    if (note.valeur < 0 || note.valeur > 20) {
        return false;
    }
    if (note.date.length() != 10 ||
        note.date[4] != '-' ||
        note.date[7] != '-') {
        return false;
    }
    if (note.matiere.empty()) {
        return false;
    }
    return true;
}

bool DonneesTest::validerAbsence(const Absence& absence) {
    if (absence.date.length() != 10 ||
        absence.date[4] != '-' ||
        absence.date[7] != '-') {
        return false;
    }
    if (absence.justifie && absence.motif.empty()) {
        return false;
    }
    return true;
}

