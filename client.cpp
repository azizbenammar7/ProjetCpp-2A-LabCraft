#include "client.h"
#include "customsqlmodel.h"
#include <QSqlQuery>
#include <QDebug>

Client::Client(int id, QString sexe, QString nom, QString prenom, QString adresse, QString email, QString dateNaissance, QString dateInscription,QByteArray image)
{
    this->id = id;
    this->sexe = sexe;
    this->nom = nom;
    this->prenom = prenom;
    this->adresse = adresse;
    this->email = email;
    this->dateNaissance = dateNaissance;
    this->dateInscription = dateInscription;
    this->image = image;

}

bool Client::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO clients (ID, nom, prenom, sexe, adresse, email, date_de_naissance, date_dinscription, image) "
                  "VALUES (:id, :nom, :prenom, :sexe, :adresse, :email, :dateNaissance, :dateInscription, :image)");
    query.bindValue(":id", id);
    query.bindValue(":sexe", sexe);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
   query.bindValue(":dateNaissance", dateNaissance);
   query.bindValue(":dateInscription", dateInscription);
   query.bindValue(":image", image);

    return query.exec();
}

QSqlQueryModel* Client::afficher()
{
    CustomSqlModel* model = new CustomSqlModel();
    model->setQuery("SELECT ID, nom, prenom, sexe, adresse, email, date_de_naissance, date_dinscription, image FROM clients");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date de naissance"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date d'inscription"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Image"));

    return model;
}

bool Client::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM clients WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}
bool Client::modifier(int id, const QString &nom, const QString &prenom, const QString &sexe, const QString &email, const QString &adresse, const QString &dateNaissance, const QString &dateInscription) {

    QSqlQuery query;
    query.prepare("UPDATE CLIENTS SET NOM = :nom , PRENOM= :prenom, SEXE= :sexe,ADRESSE = :adresse, EMAIL = :email, DATE_DE_NAISSANCE = :dateNaissance, DATE_DINSCRIPTION = :dateInscription, IMAGE= :image WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);

    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":dateInscription", dateInscription);
    query.bindValue(":image", image);

    return query.exec();
}

QSqlQueryModel* Client::chercherClient(const QString &nom) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT sexe, nom, prenom, adresse, email, date_de_naissance,date_dinscription FROM clients WHERE nom LIKE :nom");
    query.bindValue(":nom", "%" + nom + "%");
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête de recherche.";
        return nullptr;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date de naissance"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date d'inscription"));

    return model;
}
QSqlQueryModel* Client::trierClientsParDateInscription() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, nom, prenom, sexe, adresse, email, date_de_naissance, date_dinscription FROM clients ORDER BY date_dinscription DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date de naissance"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date d'inscription"));
    return model;
}
