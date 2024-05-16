#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class employe
{
    QString nom, prenom, ddn, poste, mail;
    QString mdp, question, reponse; // Nouvelles colonnes pour le mot de passe, la question de sécurité et la réponse
    int id_employe, cin, salaire;

public:
    employe(){}
    employe(int, int, QString, QString, QString, QString, QString, QString, QString, QString, int);

    QString getNom() { return nom; }
    QString getPrenom() { return prenom; }
    QString getDDN() { return ddn; }
    QString getPoste() { return poste; }
    QString getMail() { return mail; }
    QString getMDP() { return mdp; } // Getter pour le mot de passe
    QString getQuestion() { return question; } // Getter pour la question de sécurité
    QString getReponse() { return reponse; } // Getter pour la réponse de sécurité

    int getID() { return id_employe; }
    int getCin() { return cin; }
    int getSalaire() { return salaire; }

    void setNom(QString n) { nom = n; }
    void setPrenom(QString e) { prenom = e; }
    void setDDN(QString dd) { ddn = dd; }
    void setPoste(QString df) { poste = df; }
    void setMail(QString df) { mail = df; }
    void setMDP(QString pw) { mdp = pw; } // Setter pour le mot de passe
    void setQuestion(QString q) { question = q; } // Setter pour la question de sécurité
    void setReponse(QString r) { reponse = r; } // Setter pour la réponse de sécurité

    void setID(int id) { id_employe = id; }
    void setCin(int b) { cin = b; }
    void setSalaire(int b) { salaire = b; }

    bool ajouter_employe();
    QSqlQueryModel* afficher_employe();
    bool supprimer_employe(int);
    bool modifier_employe(int id, int cin, QString nom, QString prenom, QString ddn, QString poste, QString mail, QString mdp, QString question, QString reponse, int salaire);

    bool verifierConnexion(QString email, QString password, QString &poste);
    bool modifierMotDePasse(QString email, QString questionUtilisateur, QString reponseUtilisateur, QString nouveauMdp);
    static QSqlQuery obtenirRepartitionPostesQuery();
    static QSqlQuery obtenirStatistiquesPostesQuery();
    bool verifierExistenceEmployeParID(int id);
};

#endif // EMPLOYE_H
