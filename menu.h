// menu.h
#ifndef MENU_H
#define MENU_H

#include "donnees.h"
#include "auth.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>

class Menu {
public:
    static void afficherMenuPrincipal(const std::string& role, const std::string& email);

private:
    // Admin menus
    static void menuAdmin();
    static void gererEtudiants();
    static void gererProfesseurs();
    static void afficherTousEtudiants();
    static void modifierEtudiant(Etudiant* etudiant);
    static void deplacerEtudiant(Etudiant* etudiant);
    static void gererCours();

    // Professor menus
    static void menuProfesseur(const std::string& email);
    static std::vector<Etudiant> getEtudiantsParProfesseur(const std::string& emailProf);
    static void ajouterNote(const std::string& emailProf);
    static void modifierNote(const std::string& emailProf);
    static void marquerAbsence(const std::string& emailProf);
    static void modifierProfesseur();
    static void supprimerProfesseur();

    // Student menu
    static void menuEtudiant(const std::string& email);
    static void afficherInfosEtudiant(const Etudiant* etudiant);
    static void afficherNotes(const std::vector<Note>& notes);
    static void afficherAbsences(const std::vector<Absence>& absences);

    // Utility functions
    static std::string genererEmail(const std::string& nom, const std::string& prenom);
    static std::string genererMotDePasse(const std::string& nom);
    static bool validateStudentId(int id);
    static bool validateEmail(const std::string& email);
    static std::string sanitizeInput(const std::string& input);

    static void ajouterEtudiant();
    static void ajouterProfesseur();
    static void gererFilieres();
    static void gererGroupes();
    static void afficherEtudiantsParGroupe();
    static void afficherProfsParFiliere();
};

#endif
