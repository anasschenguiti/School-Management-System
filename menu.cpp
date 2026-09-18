// menu.cpp
#include "menu.h"
#include "donnees.h"
#include "auth.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <random>
#include <regex>

void Menu::afficherMenuPrincipal(const std::string& role, const std::string& email) {
    DonneesTest::chargerDonnees();
    std::cout << "\n=== Menu Principal ===\n";

    if (role == "student") {
        menuEtudiant(email);
    } else if (role == "professor") {
        menuProfesseur(email);
    } else if (role == "admin") {
        menuAdmin();
    }
}

void Menu::menuEtudiant(const std::string& email) {
    try {
        Etudiant* etudiant = DonneesTest::getEtudiantParEmail(email);
        if (!etudiant) {
            throw std::runtime_error("Etudiant non trouve.");
        }

        while (true) {
            std::cout << "\n=== Menu Etudiant ===\n"
                     << "1. Mes informations\n"
                     << "2. Mes notes\n"
                     << "3. Mes absences\n"
                     << "4. Se deconnecter\n"
                     << "Choix: ";

            std::string choix;
            std::getline(std::cin, choix);
            try {
                if (choix == "1") {
                    afficherInfosEtudiant(etudiant);
                } else if (choix == "2") {
                    afficherNotes(etudiant->notes);
                } else if (choix == "3") {
                    afficherAbsences(etudiant->absences);
                } else if (choix == "4") {
                    break;
                }
            } catch (const std::exception& e) {
                std::cout << "Erreur: " << e.what() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Erreur: " << e.what() << "\n";
    }
}

void Menu::ajouterEtudiant() {
    std::cout << "\n=== Ajouter un etudiant ===\n";
    std::string nom, prenom, filiere, groupe;

    std::cout << "Nom: ";
    std::getline(std::cin, nom);
    std::cout << "Prenom: ";
    std::getline(std::cin, prenom);

    // Afficher les filières disponibles
    const auto& filieres = DonneesTest::getFilieres();
    std::cout << "\nFilières disponibles:\n";
    for (const auto& f : filieres) {
        std::cout << "- " << f.nom << "\n";
    }
    std::cout << "Filiere: ";
    std::getline(std::cin, filiere);

    // Afficher les groupes disponibles pour cette filière
    const auto& groupes = DonneesTest::getGroupes();
    std::cout << "\nGroupes disponibles pour cette filière:\n";
    for (const auto& g : groupes) {
        if (g.filiere == filiere) {
            std::cout << "- " << g.nom << " (" << g.etudiantsIds.size() << " étudiants)\n";
        }
    }
    std::cout << "Groupe: ";
    std::getline(std::cin, groupe);

    int id = DonneesTest::getNewEtudiantId();
    Etudiant nouvelEtudiant = {
        id, nom, prenom, "", filiere, groupe, {}, {}, "photo.png"
    };

    try {
        DonneesTest::ajouterEtudiant(nouvelEtudiant);

        // Mettre ŕ jour le groupe avec le nouvel étudiant
        auto& groupesModifiables = DonneesTest::getGroupes();
        for (auto& g : groupesModifiables) {
            if (g.nom == groupe && g.filiere == filiere) {
                g.etudiantsIds.push_back(id);
                break;
            }
        }
        DonneesTest::sauvegarderGroupes();

        std::cout << "Etudiant ajouté avec succès.\n";
    } catch (const std::exception& e) {
        std::cout << "Erreur lors de l'ajout de l'étudiant: " << e.what() << "\n";
    }
}


void Menu::gererEtudiants() {
    while (true) {
        std::cout << "\n=== Gestion des Etudiants ===\n"
                  << "1. Afficher tous les etudiants\n"
                  << "2. Modifier un etudiant\n"
                  << "3. Deplacer un etudiant\n"
                  << "4. Supprimer un etudiant\n"
                  << "5. Ajouter un etudiant\n"
                  << "6. Retour\n"
                  << "Choix: ";

        std::string choix;
        std::getline(std::cin, choix);

        try {
            if (choix == "1") {
                afficherTousEtudiants();
            } else if (choix == "2") {
                std::cout << "ID de l'etudiant: ";
                std::string idStr;
                std::getline(std::cin, idStr);
                int id = std::stoi(idStr);
                Etudiant* etudiant = DonneesTest::getEtudiantParId(id);
                if (etudiant) {
                    modifierEtudiant(etudiant);
                } else {
                    std::cout << "Etudiant non trouve.\n";
                }
            } else if (choix == "3") {
                std::cout << "ID de l'etudiant: ";
                std::string idStr;
                std::getline(std::cin, idStr);
                int id = std::stoi(idStr);
                Etudiant* etudiant = DonneesTest::getEtudiantParId(id);
                if (etudiant) {
                    deplacerEtudiant(etudiant);
                } else {
                    std::cout << "Etudiant non trouve.\n";
                }
            } else if (choix == "4") {
                std::cout << "ID de l'etudiant: ";
                std::string idStr;
                std::getline(std::cin, idStr);
                int id = std::stoi(idStr);
                DonneesTest::supprimerEtudiant(id);
                std::cout << "Etudiant supprime avec succes.\n";
            } else if (choix == "5") {
                ajouterEtudiant();
            } else if (choix == "6") {
                break;
            }
        } catch (const std::exception& e) {
            std::cout << "Erreur: " << e.what() << "\n";
        }
    }
}

void Menu::afficherInfosEtudiant(const Etudiant* etudiant) {
    if (etudiant) {
        std::cout << "\nInformations de l'etudiant:\n";
        std::cout << "ID: " << etudiant->id << "\n";
        std::cout << "Nom: " << etudiant->nom << "\n";
        std::cout << "Prenom: " << etudiant->prenom << "\n";
        std::cout << "Email: " << etudiant->email << "\n";
        std::cout << "Filiere: " << etudiant->filiere << "\n";
        std::cout << "Groupe: " << etudiant->groupe << "\n";
        std::cout << "Photo: " << etudiant->photoPath << "\n";
    } else {
        std::cout << "Etudiant non trouve.\n";
    }
}

void Menu::afficherTousEtudiants() {
    const auto& etudiants = DonneesTest::getEtudiants();
    std::cout << "\nListe de tous les etudiants:\n";
    std::cout << std::setw(5) << "ID"
              << std::setw(20) << "Nom"
              << std::setw(20) << "Prenom"
              << std::setw(30) << "Email" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (const auto& etudiant : etudiants) {
        std::cout << std::setw(5) << etudiant.id
                  << std::setw(20) << etudiant.nom
                  << std::setw(20) << etudiant.prenom
                  << std::setw(30) << etudiant.email << std::endl;
    }
}

void Menu::modifierEtudiant(Etudiant* etudiant) {
    if (etudiant) {
        std::string newNom, newPrenom, newEmail, newFiliere, newGroupe;
        std::cout << "Nouveau nom: ";
        std::getline(std::cin, newNom);
        std::cout << "Nouveau prenom: ";
        std::getline(std::cin, newPrenom);
        std::cout << "Nouvel email: ";
        std::getline(std::cin, newEmail);
        std::cout << "Nouvelle filiere: ";
        std::getline(std::cin, newFiliere);
        std::cout << "Nouveau groupe: ";
        std::getline(std::cin, newGroupe);

        etudiant->nom = newNom;
        etudiant->prenom = newPrenom;
        etudiant->email = newEmail;
        etudiant->filiere = newFiliere;
        etudiant->groupe = newGroupe;

        std::cout << "Informations de l'etudiant mises a jour avec succes.\n";
    } else {
        std::cout << "Etudiant non trouve.\n";
    }
}

void Menu::deplacerEtudiant(Etudiant* etudiant) {
    if (etudiant) {
        std::string newFiliere, newGroupe;
        std::cout << "Nouvelle filiere: ";
        std::getline(std::cin, newFiliere);
        std::cout << "Nouveau groupe: ";
        std::getline(std::cin, newGroupe);

        etudiant->filiere = newFiliere;
        etudiant->groupe = newGroupe;

        std::cout << "Etudiant deplace avec succes.\n";
    } else {
        std::cout << "Etudiant non trouve.\n";
    }
}

std::vector<Etudiant> Menu::getEtudiantsParProfesseur(const std::string& emailProf) {
    std::vector<Etudiant> etudiants;
    std::vector<std::string> profCourses;

    for (const auto& cours : DonneesTest::getCours()) {
        if (cours.professeur == emailProf) {
            profCourses.push_back(cours.code);
        }
    }

    if (profCourses.empty()) {
        std::cout << "Aucun cours trouvé pour ce professeur.\n";
        return etudiants;
    }

    for (const auto& filiere : DonneesTest::getFilieres()) {
        bool hasProfCourse = std::any_of(profCourses.begin(), profCourses.end(),
            [&filiere](const std::string& course) {
                return std::find(filiere.cours.begin(), filiere.cours.end(), course)
                       != filiere.cours.end();
            });

        if (hasProfCourse) {
            auto filierEtudiants = DonneesTest::getEtudiantsParFiliere(filiere.nom);
            etudiants.insert(etudiants.end(), filierEtudiants.begin(), filierEtudiants.end());
        }
    }

    std::sort(etudiants.begin(), etudiants.end(),
              [](const Etudiant& a, const Etudiant& b) { return a.id < b.id; });
    etudiants.erase(
        std::unique(etudiants.begin(), etudiants.end(),
                    [](const Etudiant& a, const Etudiant& b) { return a.id == b.id; }),
        etudiants.end());

    return etudiants;
}

void Menu::afficherEtudiantsParGroupe() {
    std::cout << "\n=== Etudiants par groupe ===\n";

    std::cout << "Nom du groupe: ";
    std::string groupe;
    std::getline(std::cin, groupe);

    auto etudiants = DonneesTest::getEtudiantsParGroupe(groupe);
    if (etudiants.empty()) {
        std::cout << "Aucun etudiant trouve dans ce groupe.\n";
        return;
    }

    std::cout << "\nListe des etudiants du groupe " << groupe << ":\n";
    std::cout << std::setw(5) << "ID"
              << std::setw(20) << "Nom"
              << std::setw(20) << "Prenom"
              << std::setw(30) << "Email" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (const auto& etudiant : etudiants) {
        std::cout << std::setw(5) << etudiant.id
                  << std::setw(20) << etudiant.nom
                  << std::setw(20) << etudiant.prenom
                  << std::setw(30) << etudiant.email << std::endl;
    }
}

void Menu::menuProfesseur(const std::string& email) {
    try {
        auto etudiants = getEtudiantsParProfesseur(email);
        if (etudiants.empty()) {
            throw std::runtime_error("Aucun etudiant n'est affecte a vos cours.");
        }

        // Récupérer les matières enseignées par le professeur
        std::vector<std::string> matieresProf;
        for (const auto& cours : DonneesTest::getCours()) {
            if (cours.professeur == email) {
                matieresProf.push_back(cours.code);
            }
        }

        while (true) {
            std::cout << "\n=== Menu Professeur ===\n"
                     << "1. Voir mes etudiants\n"
                     << "2. Ajouter une note\n"
                     << "3. Modifier une note\n"
                     << "4. Marquer une absence\n"
                     << "5. Se deconnecter\n"
                     << "Choix: ";

            std::string choix;
            std::getline(std::cin, choix);

            try {
                   if (choix == "1") {
                    std::cout << "\n=== Liste detaillee des etudiants ===\n";
                    for (const auto& etudiant : etudiants) {
                        // Afficher les informations de base
                        std::cout << "\n----------------------------------------\n";
                        std::cout << "ID: " << etudiant.id << "\n";
                        std::cout << "Nom complet: " << etudiant.nom << " " << etudiant.prenom << "\n";
                        std::cout << "Email: " << etudiant.email << "\n";
                        std::cout << "Filiere: " << etudiant.filiere << "\n";
                        std::cout << "Groupe: " << etudiant.groupe << "\n";

                        // Afficher les notes pour les matières du prof
                        std::cout << "\nNotes dans vos cours:\n";
                        for (const auto& note : etudiant.notes) {
                            if (std::find(matieresProf.begin(), matieresProf.end(), note.matiere) != matieresProf.end()) {
                                std::cout << "- [" << note.matiere << "] " << note.valeur << "/20"
                                          << " (" << note.appreciation << ") - " << note.date << "\n";
                            }
                        }

                        // Afficher les absences pour les matières du prof
                        std::cout << "\nAbsences dans vos cours:\n";
                        for (const auto& absence : etudiant.absences) {
                            if (std::find(matieresProf.begin(), matieresProf.end(), absence.matiere) != matieresProf.end()) {
                                std::cout << "- [" << absence.matiere << "] " << absence.date
                                          << " (" << (absence.justifie ? "Justifiée" : "Non justifiée")
                                          << ") - " << absence.motif << "\n";
                            }
                        }
                        std::cout << "----------------------------------------\n";
                    }
                } else if (choix == "2") {
                    ajouterNote(email);
                } else if (choix == "3") {
                    modifierNote(email);
                } else if (choix == "4") {
                    marquerAbsence(email);
                } else if (choix == "5") {
                    break;
                }
            } catch (const std::exception& e) {
                std::cout << "Erreur: " << e.what() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Erreur: " << e.what() << "\n";
    }
}

void Menu::gererProfesseurs() {
    while (true) {
        std::cout << "\n=== Gestion des Professeurs ===\n"
                  << "1. Afficher tous les professeurs\n"
                  << "2. Ajouter un professeur\n"
                  << "3. Modifier un professeur\n"
                  << "4. Supprimer un professeur\n"
                  << "5. Voir professeurs par filiere\n"
                  << "6. Retour\n"
                  << "Choix: ";

        std::string choix;
        std::getline(std::cin, choix);

        try {
            if (choix == "1") {
                const auto& cours = DonneesTest::getCours();
                std::cout << "\nListe des professeurs par cours:\n";
                for (const auto& c : cours) {
                    if (!c.professeur.empty()) {
                        std::cout << "Cours: " << c.code << " - " << c.nom
                                  << "\nProfesseur: " << c.professeur << "\n\n";
                    }
                }
            } else if (choix == "2") {
                ajouterProfesseur();
            } else if (choix == "3") {
                modifierProfesseur();
            } else if (choix == "4") {
                supprimerProfesseur();
            } else if (choix == "5") {
                afficherProfsParFiliere();
            } else if (choix == "6") {
                break;
            }
        } catch (const std::exception& e) {
            std::cout << "Erreur: " << e.what() << "\n";
        }
    }
}

void Menu::modifierProfesseur() {
    // Afficher d'abord tous les cours et leurs professeurs
    std::cout << "\nListe des cours et leurs professeurs:\n";
    auto& cours = DonneesTest::getCours();
    for (const auto& c : cours) {
        std::cout << "Code: " << c.code
                  << " - Nom: " << c.nom
                  << " - Professeur: " << (c.professeur.empty() ? "Non assigné" : c.professeur)
                  << "\n";
    }

    std::string codeCoursAModifier;
    std::cout << "\nEntrez le code du cours à modifier: ";
    std::getline(std::cin, codeCoursAModifier);

    auto coursIt = std::find_if(cours.begin(), cours.end(),
                               [&codeCoursAModifier](const Cours& c) {
                                   return c.code == codeCoursAModifier;
                               });

    if (coursIt == cours.end()) {
        std::cout << "Cours non trouvé.\n";
        return;
    }

    std::cout << "Nouveau professeur (email) ou vide pour retirer l'assignation: ";
    std::string newEmail;
    std::getline(std::cin, newEmail);

    if (!newEmail.empty() && !validateEmail(newEmail)) {
        std::cout << "Format d'email invalide.\n";
        return;
    }

    coursIt->professeur = newEmail;
    DonneesTest::sauvegarderCours();
    std::cout << "Professeur " << (newEmail.empty() ? "retiré" : "modifié") << " avec succès.\n";
}

 void Menu::ajouterProfesseur() {
    std::cout << "\n=== Ajouter un professeur ===\n";
    std::string nom, prenom;
    std::cout << "Nom: ";
    std::getline(std::cin, nom);
    std::cout << "Prénom: ";
    std::getline(std::cin, prenom);

    // Générer l'email
    std::string email = genererEmail(nom, prenom);
    std::string password = genererMotDePasse(nom);

    // Récupérer la liste des cours sans professeur
    const auto& coursList = DonneesTest::getCours();
    std::vector<std::string> coursDisponibles;

    for (const auto& cours : coursList) {
        if (cours.professeur.empty()) {
            coursDisponibles.push_back(cours.code);
        }
    }

    if (coursDisponibles.empty()) {
        std::cout << "Aucun cours disponible pour assignation.\n";
        return;
    }

    // Afficher les cours disponibles
    std::cout << "\nCours disponibles:\n";
    for (const auto& coursCode : coursDisponibles) {
        for (const auto& cours : coursList) {
            if (cours.code == coursCode) {
                std::cout << cours.code << " - " << cours.nom << "\n";
                break;
            }
        }
    }

    // Variable pour suivre si au moins un cours a été assigné
    bool coursAssigne = false;
    std::vector<std::string> coursChoisis;

    // Assigner des cours au professeur
    while (true) {
        std::cout << "\nCode du cours à assigner (ou vide pour terminer): ";
        std::string coursCode;
        std::getline(std::cin, coursCode);

        if (coursCode.empty()) {
            if (!coursAssigne) {
                std::cout << "Vous devez assigner au moins un cours au professeur.\n";
                continue;
            }
            break;
        }

        // Vérifier si le cours existe et est disponible
        if (std::find(coursDisponibles.begin(), coursDisponibles.end(), coursCode) == coursDisponibles.end()) {
            std::cout << "Cours non disponible ou invalide.\n";
            continue;
        }

        coursChoisis.push_back(coursCode);
        coursAssigne = true;
        std::cout << "Cours assigné avec succès.\n";
    }

    // Créer le compte seulement si au moins un cours a été assigné
    if (coursAssigne) {
        try {

            // Si la création du compte réussit, mettre à jour les cours
            auto& coursModifiables = DonneesTest::getCours();
            for (auto& cours : coursModifiables) {
                if (std::find(coursChoisis.begin(), coursChoisis.end(), cours.code) != coursChoisis.end()) {
                    cours.professeur = email;
                }
            }

            // Sauvegarder les modifications
            DonneesTest::sauvegarderCours();
            std::cout << "\nProfesseur ajouté avec succès.\n";
            std::cout << "Email: " << email << "\n";
            std::cout << "Mot de passe: " << password << "\n";
        } catch (const AuthException& e) {
            std::cout << "Erreur lors de la création du compte: " << e.what() << "\n";
        }
    }
}

void Menu::supprimerProfesseur() {
    std::cout << "Email du professeur: ";
    std::string email;
    std::getline(std::cin, email);

    if (!validateEmail(email)) {
        std::cout << "Format d'email invalide.\n";
        return;
    }

    bool found = false;
    auto& cours = DonneesTest::getCours();
    for (auto& c : cours) {
        if (c.professeur == email) {
            c.professeur = "";
            found = true;
        }
    }

    if (found) {
        DonneesTest::sauvegarderCours();
        std::cout << "Professeur supprimé des cours avec succès.\n";
    } else {
        std::cout << "Professeur non trouvé.\n";
    }

    std::cout << "Note: La suppression du compte d'authentification doit être gérée séparément.\n";
}

void Menu::afficherProfsParFiliere() {
    const auto& filieres = DonneesTest::getFilieres();

    std::cout << "\nListe des filières disponibles:\n";
    for (const auto& filiere : filieres) {
        std::cout << "- " << filiere.nom << "\n";
    }

    std::cout << "\nEntrez le nom de la filière: ";
    std::string filiere;
    std::getline(std::cin, filiere);

    auto it = std::find_if(filieres.begin(), filieres.end(),
                          [&filiere](const Filiere& f) { return f.nom == filiere; });

    if (it == filieres.end()) {
        std::cout << "Filière non trouvée.\n";
        return;
    }

    std::cout << "\nProfesseurs de la filière " << filiere << ":\n";
    std::cout << std::setw(20) << "Cours"
              << std::setw(30) << "Code"
              << std::setw(30) << "Professeur" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (const auto& codeCours : it->cours) {
        for (const auto& cours : DonneesTest::getCours()) {
            if (cours.code == codeCours) {
                std::cout << std::setw(20) << cours.nom
                          << std::setw(30) << cours.code
                          << std::setw(30) << (cours.professeur.empty() ? "Non assigné" : cours.professeur)
                          << std::endl;
                break;
            }
        }
    }
}

void Menu::afficherNotes(const std::vector<Note>& notes) {
    std::cout << "\nListe des notes:\n";
    std::cout << std::setw(15) << "Matiere"
              << std::setw(10) << "Note"
              << std::setw(20) << "Appreciation"
              << std::setw(15) << "Date" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto& note : notes) {
        std::cout << std::setw(15) << note.matiere
                  << std::setw(10) << note.valeur
                  << std::setw(20) << note.appreciation
                  << std::setw(15) << note.date << std::endl;
    }
}

void Menu::ajouterNote(const std::string& emailProf) {
    auto etudiants = getEtudiantsParProfesseur(emailProf);

    std::cout << "\nAjouter une note\n";
    std::cout << "ID de l'etudiant: ";
    std::string idStr;
    std::getline(std::cin, idStr);
    int id;
    try {
        id = std::stoi(idStr);
    } catch (const std::exception& e) {
        std::cout << "ID invalide.\n";
        return;
    }

    auto it = std::find_if(etudiants.begin(), etudiants.end(),
                          [id](const Etudiant& e) { return e.id == id; });

    if (it == etudiants.end()) {
        std::cout << "Cet etudiant n'est pas dans vos classes.\n";
        return;
    }
    std::cout << "Valeur de la note: ";
    std::string noteStr;
    std::getline(std::cin, noteStr);
    float note;
    try {
        note = std::stof(noteStr);
        if (note < 0 || note > 20) {
            std::cout << "La note doit etre entre 0 et 20.\n";
            return;
        }
    } catch (const std::exception& e) {
        std::cout << "Note invalide.\n";
        return;
    }

    std::cout << "Appreciation: ";
    std::string appreciation;
    std::getline(std::cin, appreciation);

    std::cout << "Date (YYYY-MM-DD): ";
    std::string date;
    std::getline(std::cin, date);

    // Trouver le cours du professeur
    std::string matiere;
    for (const auto& cours : DonneesTest::getCours()) {
        if (cours.professeur == emailProf) {
            matiere = cours.code;
            break;
        }
    }

    Note nouvelleNote = {note, appreciation, date, matiere};

    if (DonneesTest::ajouterNote(id, nouvelleNote)) {
        std::cout << "Note ajoutee avec succes.\n";
    } else {
        std::cout << "Erreur lors de l'ajout de la note.\n";
    }
}

void Menu::modifierNote(const std::string& emailProf) {
    auto etudiants = getEtudiantsParProfesseur(emailProf);

    std::cout << "\nModifier une note\n";
    std::cout << "ID de l'etudiant: ";
    std::string idStr;
    std::getline(std::cin, idStr);
    int id;
    try {
        id = std::stoi(idStr);
    } catch (const std::exception& e) {
        std::cout << "ID invalide.\n";
        return;
    }

    // Vérifier si l'étudiant fait partie des étudiants du professeur
    auto it = std::find_if(etudiants.begin(), etudiants.end(),
                          [id](const Etudiant& e) { return e.id == id; });

    if (it == etudiants.end()) {
        std::cout << "Cet etudiant n'est pas dans vos classes.\n";
        return;
    }

    std::cout << "Nouvelle note: ";
    std::string noteStr;
    std::getline(std::cin, noteStr);
    float note;
    try {
        note = std::stof(noteStr);
        if (note < 0 || note > 20) {
            std::cout << "La note doit ętre entre 0 et 20.\n";
            return;
        }
    } catch (const std::exception& e) {
        std::cout << "Note invalide.\n";
        return;
    }

    // Trouver le cours du professeur
    std::string matiere;
    for (const auto& cours : DonneesTest::getCours()) {
        if (cours.professeur == emailProf) {
            matiere = cours.code;
            break;
        }
    }

    if (DonneesTest::modifierNote(id, matiere, note)) {
        std::cout << "Note modifiee avec succes.\n";
    } else {
        std::cout << "Erreur lors de la modification de la note.\n";
    }
}

void Menu::afficherAbsences(const std::vector<Absence>& absences) {
    std::cout << "\nListe des absences:\n";
    std::cout << std::setw(15) << "Date"
              << std::setw(15) << "Matiere"
              << std::setw(10) << "Justifiee"
              << std::setw(20) << "Motif" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto& absence : absences) {
        std::cout << std::setw(15) << absence.date
                  << std::setw(15) << absence.matiere
                  << std::setw(10) << (absence.justifie ? "Oui" : "Non")
                  << std::setw(20) << absence.motif << std::endl;
    }
}


void Menu::marquerAbsence(const std::string& emailProf) {
    auto etudiants = getEtudiantsParProfesseur(emailProf);

    std::cout << "\nMarquer une absence\n";
    std::cout << "ID de l'etudiant: ";
    std::string idStr;
    std::getline(std::cin, idStr);
    int id;
    try {
        id = std::stoi(idStr);
    } catch (const std::exception& e) {
        std::cout << "ID invalide.\n";
        return;
    }

    // Vérifier si l'étudiant fait partie des étudiants du professeur
    auto it = std::find_if(etudiants.begin(), etudiants.end(),
                          [id](const Etudiant& e) { return e.id == id; });

    if (it == etudiants.end()) {
        std::cout << "Cet etudiant n'est pas dans vos classes.\n";
        return;
    }

    std::cout << "Date (YYYY-MM-DD): ";
    std::string date;
    std::getline(std::cin, date);

    std::cout << "Justifiee (o/n): ";
    std::string justifieStr;
    std::getline(std::cin, justifieStr);
    bool justifie = (justifieStr == "o" || justifieStr == "O");

    std::string motif;
    if (justifie) {
        std::cout << "Motif: ";
        std::getline(std::cin, motif);
    }

    // Trouver le cours du professeur
    std::string matiere;
    for (const auto& cours : DonneesTest::getCours()) {
        if (cours.professeur == emailProf) {
            matiere = cours.code;
            break;
        }
    }

    Absence absence = {
        date,
        justifie,
        motif,
        matiere
    };

    if (DonneesTest::ajouterAbsence(id, absence)) {
        std::cout << "Absence marquee avec succes.\n";
    } else {
        std::cout << "Erreur lors du marquage de l'absence.\n";
    }
}

void Menu::menuAdmin() {
    while (true) {
        std::string choix;
        std::cout << "\n=== Menu Admin ===\n"
                  << "1. Gestion des etudiants\n"
                  << "2. Gestion des professeurs\n"
                  << "3. Gestion des filieres\n"
                  << "4. Gestion des groupes\n"
                  << "5. Gestion des cours\n"
                  << "6. Se deconnecter\n"
                  << "Choix: ";

        std::getline(std::cin, choix);

        if (choix == "1") {
            gererEtudiants();
        } else if (choix == "2") {
            gererProfesseurs();
        } else if (choix == "3") {
            gererFilieres();
        } else if (choix == "4") {
            gererGroupes();
        }  else if (choix == "5") {
            gererCours();
        } else if (choix == "6") {
            break;
        }
    }
}


std::string Menu::genererEmail(const std::string& nom, const std::string& prenom) {
    std::string result;
    result.reserve(prenom.length() + nom.length() + 10); // Reserve space for the email

    // Add the first character of prenom if it exists
    if (!prenom.empty()) {
          char c = std::tolower(static_cast<unsigned char>(prenom[0]));
           result += c;
    }

    // Add lowercase alphanumeric characters from nom
    for (char c : nom) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            result.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
    }

    result += "@email.com";
    return result;
}

std::string Menu::genererMotDePasse(const std::string& nom) {
 // No transformation needed here
    return nom + "123";
}

void Menu::gererFilieres() {
    while (true) {
        std::cout << "\n=== Gestion des Filieres ===\n"
                  << "1. Voir les filieres\n"
                  << "2. Ajouter une filiere\n"
                  << "3. Supprimer une filiere\n"
                  << "4. Retour\n"
                  << "Choix: ";

        std::string choix;
        std::getline(std::cin, choix);

        if (choix == "1") {
            for (const auto& filiere : DonneesTest::getFilieres()) {
                std::cout << "\nNom: " << filiere.nom
                         << "\nDepartement: " << filiere.departement
                         << "\nCours:\n";
                for (const auto& cours : filiere.cours) {
                    std::cout << "- " << cours << "\n";
                }
            }
        } else if (choix == "2") {
            std::string nom, departement;
            std::cout << "Nom de la filiere: ";
            std::getline(std::cin, nom);
            std::cout << "Departement: ";
            std::getline(std::cin, departement);

            std::vector<std::string> cours;
            while (true) {
                std::cout << "Code du cours (ou vide pour terminer): ";
                std::string code;
                std::getline(std::cin, code);
                if (code.empty()) break;
                cours.push_back(code);
            }

            Filiere filiere = {nom, departement, cours};
            DonneesTest::ajouterFiliere(filiere);
            std::cout << "Filiere ajoutee avec succes.\n";
        } else if (choix == "3") {
            std::cout << "Nom de la filiere à supprimer: ";
            std::string nom;
            std::getline(std::cin, nom);
            DonneesTest::supprimerFiliere(nom);
            std::cout << "Filiere supprimee avec succes.\n";
        } else if (choix == "4") {
            break;
        }
    }
}

void Menu::gererGroupes() {
    while (true) {
        std::cout << "\n=== Gestion des Groupes ===\n"
                  << "1. Voir les groupes\n"
                  << "2. Ajouter un groupe\n"
                  << "3. Supprimer un groupe\n"
                  << "4. Retour\n"
                  << "Choix: ";

        std::string choix;
        std::getline(std::cin, choix);

        if (choix == "1") {
            for (const auto& groupe : DonneesTest::getGroupes()) {
                std::cout << "\nNom: " << groupe.nom
                         << "\nFiliere: " << groupe.filiere
                         << "\nNombre d'etudiants: " << groupe.etudiantsIds.size() << "\n";
            }
        } else if (choix == "2") {
            std::string nom, filiere;
            std::cout << "Nom du groupe: ";
            std::getline(std::cin, nom);
            std::cout << "Filiere: ";
            std::getline(std::cin, filiere);

            Groupe groupe = {nom, filiere, std::vector<int>()};
            DonneesTest::ajouterGroupe(groupe);
            std::cout << "Groupe ajoute avec succes.\n";
        } else if (choix == "3") {
            std::cout << "Nom du groupe à supprimer: ";
            std::string nom;
            std::getline(std::cin, nom);
            DonneesTest::supprimerGroupe(nom);
            std::cout << "Groupe supprime avec succes.\n";
        } else if (choix == "4") {
            break;
        }
    }
}

void Menu::gererCours() {
    while (true) {
        std::cout << "\n=== Gestion des Cours ===\n"
                  << "1. Voir les cours\n"
                  << "2. Ajouter un cours\n"
                  << "3. Supprimer un cours\n"
                  << "4. Retour\n"
                  << "Choix: ";
        std::string choix;
        std::getline(std::cin, choix);

        if (choix == "1") {
            const auto& cours = DonneesTest::getCours();
            for (const auto& c : cours) {
                std::cout << "Code: " << c.code << " | Nom: " << c.nom
                          << " | Coefficient: " << c.coefficient
                          << " | Salle: " << c.salle
                          << " | Professeur: " << (c.professeur.empty() ? "Non assigné" : c.professeur) << "\n";
            }
        } else if (choix == "2") {
            Cours nouveauCours;
            std::cout << "Code du cours: ";
            std::getline(std::cin, nouveauCours.code);
            std::cout << "Nom du cours: ";
            std::getline(std::cin, nouveauCours.nom);
            std::cout << "Coefficient: ";
            std::string coeffStr;
            std::getline(std::cin, coeffStr);
            nouveauCours.coefficient = std::stoi(coeffStr);
            std::cout << "Salle: ";
            std::getline(std::cin, nouveauCours.salle);
            nouveauCours.professeur = ""; // Par défaut, aucun professeur

            DonneesTest::ajouterCours(nouveauCours);
            std::cout << "Cours ajouté avec succès.\n";
        } else if (choix == "3") {
            std::cout << "Code du cours à supprimer: ";
            std::string code;
            std::getline(std::cin, code);
            DonneesTest::supprimerCours(code);
            std::cout << "Cours supprimé avec succès.\n";
        } else if (choix == "4") {
            break;
        }
    }
}




bool Menu::validateStudentId(int id) {
    Etudiant* etudiant = DonneesTest::getEtudiantParId(id);
    if (!etudiant) {
        std::cout << "Erreur: Etudiant non trouvé.\n";
        return false;
    }
    return true;
}

// Améliorer la validation des entrées utilisateur
bool Menu::validateEmail(const std::string& email) {
    // Expression régulière pour valider l'email
    const std::regex pattern
        ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

// Nettoyer les entrées utilisateur
std::string Menu::sanitizeInput(const std::string& input) {
    std::string clean;
    std::copy_if(input.begin(), input.end(), std::back_inserter(clean),
                 [](char c) { return std::isalnum(c) || c == '@' || c == '.' || c == '_' || c == '-'; });
    return clean;
}
