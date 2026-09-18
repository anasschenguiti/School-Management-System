#ifndef DONNEES_H
#define DONNEES_H

#include <string>
#include <vector>

struct Note {
    float valeur;
    std::string appreciation;
    std::string date;
    std::string matiere;
};

struct Absence {
    std::string date;
    bool justifie;
    std::string motif;
    std::string matiere;
};

struct Etudiant {
    int id;
    std::string nom;
    std::string prenom;
    std::string email;
    std::string filiere;
    std::string groupe;
    std::vector<Note> notes;
    std::vector<Absence> absences;
    std::string photoPath;
};

struct Cours {
    std::string code;
    std::string nom;
    int coefficient;
    std::string salle;
    std::string professeur;
};

struct Filiere {
    std::string nom;
    std::string departement;
    std::vector<std::string> cours;
};

struct Groupe {
    std::string nom;
    std::string filiere;
    std::vector<int> etudiantsIds;
};

class DonneesTest {
public:
    static void chargerDonnees();
    static void sauvegarderDonnees();
    static void sauvegarderTout();
    static void sauvegarderCours();
    static void sauvegarderGroupes();
    // Fonctions de validation
    static bool validerNote(const Note& note);
    static bool validerAbsence(const Absence& absence);

    // Gestion des étudiants
    static Etudiant* getEtudiantParId(int id);
    static Etudiant* getEtudiantParEmail(const std::string& email);
    static std::vector<Etudiant> getEtudiantsParFiliere(const std::string& filiere);
    static std::vector<Etudiant> getEtudiantsParGroupe(const std::string& groupe);
    static bool ajouterNote(int etudiantId, const Note& note);
    static bool modifierNote(int etudiantId, const std::string& matiere, float nouvelleNote);
    static bool ajouterAbsence(int etudiantId, const Absence& absence);
    static void ajouterEtudiant(const Etudiant& etudiant);
    static void supprimerEtudiant(int id);
    static int getNewEtudiantId();

    // Accesseurs
    static const std::vector<Etudiant>& getEtudiants() { return etudiants; }
    static std::vector<Cours>& getCours() { return cours; }
    static const std::vector<Filiere>& getFilieres() { return filieres; }
    static  std::vector<Groupe>& getGroupes() { return groupes; }

    // Gestion des cours et filières
    static void ajouterCours(const Cours& cours);
    static void supprimerCours(const std::string& code);
    static void ajouterFiliere(const Filiere& filiere);
    static void supprimerFiliere(const std::string& nom);
    static void ajouterGroupe(const Groupe& groupe);
    static void supprimerGroupe(const std::string& nom);

private:
    static void chargerEtudiants();
    static void sauvegarderEtudiants();
    static void chargerCours();
    static void chargerFilieres();
    static void sauvegarderFilieres();
    static void chargerGroupes();


    static std::vector<Etudiant> etudiants;
    static std::vector<Cours> cours;
    static std::vector<Filiere> filieres;
    static std::vector<Groupe> groupes;
};

#endif
