#include "reservation.h"


reservation::reservation( QString id_equipement,QString id_projet,QDate dates ,QString  heure)

{
    this->id_projet = id_projet;
    this->id_equipement = id_equipement;
    this->dates = dates;
    this->heure = heure;



}



bool reservation::ajouter()
    {
        QSqlQuery query;
        query.prepare("INSERT INTO reservation ( id_equipement,id_projet, dates, heure) "
                      "VALUES (:id_equipement, :id_projet, :dates, :heure)");
        query.bindValue(":id_equipement", id_equipement);
        query.bindValue(":id_projet", id_projet);
        query.bindValue(":dates", dates);
        query.bindValue(":heure", heure);

        return query.exec();
    }
QSqlQueryModel * reservation::afficherreservation()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_equipement, id_projet, dates, heure FROM reservation");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_equipement"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("id_projet"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("dates"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("heure"));

    return model;
}
QList<QString> reservation::getprojetIds() {
    QList<QString> ids;
    QSqlQuery query;
    query.exec("SELECT id_projet FROM projet");
    while (query.next()) {
        ids.append(query.value(0).toString());
    }
    return ids;
}
    QList<QString> reservation::getequipementIds() {
    QList<QString> ids;
    QSqlQuery query;
    query.exec("SELECT id_equipement FROM equipement");
    while (query.next()) {
        ids.append(query.value(0).toString());
    }
    return ids;
}
    bool reservation::updateEquipmentState(const QString &state, QString &error)
    {
        QSqlQuery query;

        // Prepare the SQL update statement
        QString updateQuery = "UPDATE Equipement SET etat = :state WHERE id_equipement = :id";

        // Bind the values to the query
        query.prepare(updateQuery);
        query.bindValue(":state", state);
        query.bindValue(":id", id_equipement);

        // Execute the query
        if (query.exec()) {
            // If the query executed successfully, return true
            return true;
        } else {
            // If an error occurred, set the error message and return false
            return false;
        }
    }

