#include "salle.h"
#include <QDebug>
#include <QSqlError>


Salle::Salle(int id_salle, int capacite, double prix_jour, QString etat,double temp_max)
{
    this->id_salle = id_salle;
    this->capacite = capacite;
    this->prix_jour = prix_jour;
    this->etat = etat;
    this->temp_max = temp_max;
}

bool Salle::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO salle (id_salle, capacite, prix_jour, etat, temp_max) "
                  "VALUES (:id_salle, :capacite, :prix_jour, :etat , :temp_max)");
    query.bindValue(":id_salle", id_salle);
    query.bindValue(":capacite", capacite);
    query.bindValue(":prix_jour", prix_jour);
    query.bindValue(":etat", etat);
    query.bindValue(":temp_max", temp_max);

    return query.exec();
}

QSqlQueryModel* Salle::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_salle, capacite, prix_jour, etat ,temp_max FROM salle");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Capacite"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prix_Jour"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Temp_max"));
    return model;
}
bool Salle::supprimer(int id_salle)
{
    QSqlQuery query;
    query.prepare("DELETE FROM salle WHERE id_salle = :id_salle");
    query.bindValue(":id_salle", id_salle);

    if (query.exec() && query.numRowsAffected() > 0) {
        return true;
    } else {
        return false;
    }
}

bool Salle::modifier(int id_salle, int capacite, double prix_jour, const QString &etat,double temp_max)
{
    QSqlQuery query;
    query.prepare("UPDATE salle SET capacite = :capacite, prix_jour = :prix_jour, etat = :etat ,temp_max = :temp_max WHERE id_salle = :id_salle");
    query.bindValue(":id_salle", id_salle);
    query.bindValue(":capacite", capacite);
    query.bindValue(":prix_jour", prix_jour);
    query.bindValue(":etat", etat);
    query.bindValue(":temp_max", temp_max);

    return query.exec();
}
void Salle::getSalleEtatFromDatabase(int id_salle, QString &etat)
{
    QSqlQuery query;
    query.prepare("SELECT etat FROM salle WHERE id_salle = :id_salle");
    query.bindValue(":id_salle", id_salle);

    if (query.exec() && query.next()) {
        etat = query.value(0).toString();
    } else {
        etat = QString();
    }
}
void Salle::incrementModifications() {
    modifications++;
}


Salle Salle::getSalleFromDatabase(int id_salle) {
    Salle salle;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Erreur: Connexion à la base de données invalide";
        return salle;
    }

    QSqlQuery query(db);
    query.prepare("SELECT capacite, prix_jour, etat ,temp_max FROM salle WHERE id_salle = :id_salle");
    query.bindValue(":id_salle", id_salle);

    if (query.exec() && query.next()) {
        salle.setIdSalle(id_salle);
        salle.setCapacite(query.value(0).toInt());
        salle.setPrixJour(query.value(1).toDouble());
        salle.setEtat(query.value(2).toString());
        salle.setTemp_max(query.value(3).toDouble());

    } else {
        qDebug() << "Erreur lors de la récupération des informations de la salle :" << query.lastError().text();
    }

    return salle;
}
QList<Salle> Salle::getSallesFromDatabase() {
    QList<Salle> salles;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Erreur: Connexion à la base de données invalide";
        return salles;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id_salle, prix_jour, capacite, etat,temp_max FROM salle");

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête SQL:" << query.lastError().text();
        return salles;
    }

    while (query.next()) {
        Salle salle;
        salle.setIdSalle(query.value(0).toInt());
        salle.setPrixJour(query.value(1).toDouble());
        salle.setCapacite(query.value(2).toInt());
        salle.setEtat(query.value(3).toString());
        salle.setTemp_max(query.value(4).toDouble());
        salles.append(salle);
    }


    return salles;
}
QSqlQueryModel* Salle::trierSalleParPrix()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id_salle, capacite, prix_jour, etat FROM salle ORDER BY prix_jour DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prix par jour"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    return model;
}
QSqlQueryModel* Salle::rechercherSallesParCapacite(int capaciteMin) {
    QSqlQueryModel* modelSalle = new QSqlQueryModel();
    QSqlQuery querySalle;

    querySalle.prepare("SELECT id_salle, capacite, prix_jour, etat FROM salle WHERE capacite >= :capaciteMin");
    querySalle.bindValue(":capaciteMin", capaciteMin);

    if (!querySalle.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête de recherche par capacité.";
        return nullptr;
    }

    modelSalle->setQuery(querySalle);
    modelSalle->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    modelSalle->setHeaderData(1, Qt::Horizontal, QObject::tr("Capacité"));
    modelSalle->setHeaderData(2, Qt::Horizontal, QObject::tr("Prix par jour"));
    modelSalle->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));

    return modelSalle;
}
QSqlQueryModel* Salle::exporterSalleInfoToPDF(int id_salle) {
    QSqlQueryModel* modelSalle = new QSqlQueryModel();
    QSqlQuery querySalle;

    querySalle.prepare("SELECT capacite, prix_jour, etat, temp_max FROM salle WHERE id_salle = :id_salle");
    querySalle.bindValue(":id_salle", id_salle);

    if (!querySalle.exec()) {
        qDebug() << "Erreur lors de la récupération des informations de la salle pour l'exportation.";
        delete modelSalle;
        return nullptr;
    }

    modelSalle->setQuery(querySalle);
    return modelSalle;
}

QSqlQueryModel* Salle::getSalleStatsByEtat() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT etat, COUNT(*) FROM salle GROUP BY etat");
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête.";
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    return model;
}
QSqlQueryModel* Salle::getSalleStatByEtat() {
    QSqlQueryModel* salleStats = new QSqlQueryModel();

    QSqlQuery query;
    if (query.exec("SELECT etat, COUNT(*) FROM salle GROUP BY etat")) {
        salleStats->setQuery(query);
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête de statistiques de salles par état.";
    }

    return salleStats;
}


