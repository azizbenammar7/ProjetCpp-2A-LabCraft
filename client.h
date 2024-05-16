#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QByteArray>

class Client
{
    int id;
    QString sexe;
    QString nom;
    QString prenom;
    QString adresse;
    QString email;
    QString dateNaissance;
    QString dateInscription;
    QByteArray image;


public:
    Client() {}
    Client(int id, QString sexe, QString nom, QString prenom, QString adresse, QString email, QString dateNaissance, QString dateInscription, QByteArray image);

    int getId() const { return id; }
    QString getSexe() const { return sexe; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getAdresse() const { return adresse; }
    QString getEmail() const { return email; }
    QString getDateNaissance() const { return dateNaissance; }
   QString getDateInscription() const { return dateInscription; }
   QByteArray getImage() const { return image; }

    void setId(int id) { this->id = id; }
    void setSexe(const QString& sexe) { this->sexe = sexe; }
    void setNom(const QString& nom) { this->nom = nom; }
    void setPrenom(const QString& prenom) { this->prenom = prenom; }
    void setAdresse(const QString& adresse) { this->adresse = adresse; }
    void setEmail(const QString& email) { this->email = email; }
    void setDateNaissance(const QString& dateNaissance) { this->dateNaissance = dateNaissance; }
    void setDateInscription(const QString& dateInscription) { this->dateInscription = dateInscription; }
    void setImage(const QByteArray& image) { this->image = image; }

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    QSqlQueryModel* chercherClient(const QString &nom) ;
    QSqlQueryModel* trierClientsParDateInscription() ;

    bool modifier(int id, const QString &nom, const QString &prenom, const QString &sexe, const QString &email, const QString &adresse, const QString &dateNaissance, const QString &dateInscription);
};

#endif // CLIENT_H
