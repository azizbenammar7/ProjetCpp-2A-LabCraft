#include "formation.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QPrinter>
#include <QDate>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include <QSqlError>

Formation::Formation(QString titre, QString description, QString date_debut, QString niveau_formation, int cout, int id_employe, int id_salle, int nbr_likes, int nbr_dislikes)
{
    this->titre = titre;
    this->description = description;
    this->date_debut = date_debut;
    this->niveau_formation = niveau_formation;
    this->cout = cout;
    this->id_employe = id_employe;
    this->id_salle = id_salle;
    this->nbr_likes = nbr_likes;
    this->nbr_dislikes = nbr_dislikes;
}

bool Formation::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO formation (TITRE, DESCRIPTION, DATE_DEBUT, NIVEAU_FORMATION, COUT, ID_EMPLOYE, ID_SALLE, NBR_LIKES, NBR_DISLIKES) "
                  "VALUES (:titre, :description, :date_debut, :niveau_formation, :cout, :id_employe, :id_salle, :nbr_likes, :nbr_dislikes)");
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":niveau_formation", niveau_formation);
    query.bindValue(":cout", cout);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_salle", id_salle);
    query.bindValue(":nbr_likes", nbr_likes);
    query.bindValue(":nbr_dislikes", nbr_dislikes);

    return query.exec();
}

QSqlQueryModel * Formation::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_FORMATIONS, TITRE, DESCRIPTION, DATE_DEBUT, NIVEAU_FORMATION, COUT, ID_EMPLOYE, ID_SALLE, NBR_LIKES, NBR_DISLIKES FROM formation");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Titre"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Niveau formation"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Coût"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID Salle"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Nbr Likes"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Nbr Dislikes"));
    return model;
}

bool Formation::supprimer(int id_formations)
{
    QSqlQuery query;
    query.prepare("DELETE FROM formation WHERE ID_FORMATIONS = :id_formations");
    query.bindValue(":id_formations", id_formations);

    return query.exec();
}

bool Formation::modifier(int id_formations, QString titre, QString description, QString date_debut, QString niveau_formation, int cout, int id_employe, int id_salle, int nbr_likes, int nbr_dislikes)
{
    QSqlQuery query;
    query.prepare("UPDATE formation SET TITRE = :titre, DESCRIPTION = :description, DATE_DEBUT = :date_debut, NIVEAU_FORMATION = :niveau_formation, COUT = :cout, ID_EMPLOYE = :id_employe, ID_SALLE = :id_salle, NBR_LIKES = :nbr_likes, NBR_DISLIKES = :nbr_dislikes WHERE ID_FORMATIONS = :id_formations");
    query.bindValue(":id_formations", id_formations);
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":niveau_formation", niveau_formation);
    query.bindValue(":cout", cout);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_salle", id_salle);
    query.bindValue(":nbr_likes", nbr_likes);
    query.bindValue(":nbr_dislikes", nbr_dislikes);

    return query.exec();
}


QList<int> Formation::getEmployeeIds()
{
    QList<int> employeeIds;
    QSqlQuery query;
    query.exec("SELECT ID_EMPLOYE FROM employe");
    while (query.next()) {
        int id = query.value(0).toInt();
        employeeIds.append(id);
    }
    return employeeIds;
}

QList<int> Formation::getSalleIds()
{
    QList<int> salleIds;
    QSqlQuery query;
    query.exec("SELECT ID_SALLE FROM salle");
    while (query.next()) {
        int id = query.value(0).toInt();
        salleIds.append(id);
    }
    return salleIds;
}


QSqlQueryModel* Formation::filtrerFormationsNextWeek() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query = "SELECT * FROM \"KARIM\".\"FORMATION\" WHERE TO_DATE(\"DATE_DEBUT\", 'DD/MM/YYYY') BETWEEN SYSDATE AND SYSDATE + 7;";
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Formation::getSortedFormationsByCout()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_FORMATIONS, TITRE, DESCRIPTION, DATE_DEBUT, NIVEAU_FORMATION, COUT, ID_EMPLOYE, ID_SALLE, NBR_LIKES, NBR_DISLIKES FROM formation ORDER BY COUT ASC");
    return model;
}

QSqlQueryModel* Formation::searchFormationsByTitre(const QString& titre)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = QString("SELECT ID_FORMATIONS, TITRE, DESCRIPTION, DATE_DEBUT, NIVEAU_FORMATION, COUT, ID_EMPLOYE, ID_SALLE, NBR_LIKES, NBR_DISLIKES FROM formation WHERE TITRE LIKE '%%1%'").arg(titre);
    model->setQuery(queryStr);
    return model;
}


QSqlQueryModel* Formation::getFormationsCountByMonth()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(TO_DATE(DATE_DEBUT, 'DD-MM-YYYY'), 'MM') AS MONTH, COUNT(*) AS FORMATION_COUNT FROM FORMATION GROUP BY TO_CHAR(TO_DATE(DATE_DEBUT, 'DD-MM-YYYY'), 'MM') ORDER BY MONTH;");
    return model;
}



QList<QDate> Formation::getFormationDates() {
    QList<QDate> dates;
    QSqlQuery query("SELECT DATE_DEBUT FROM formation");
    while (query.next()) {
        QString dateString = query.value(0).toString();
        QDate date = QDate::fromString(dateString, "dd-MM-yyyy");
        dates.append(date);
    }
    return dates;
}


QSqlQueryModel* Formation::getFormationDetailsByDate(const QDate& date)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    // Assuming the date format in your database is "dd-MM-yyyy"
    query.prepare("SELECT * FROM formation WHERE TO_DATE(DATE_DEBUT, 'DD-MM-YYYY') = TO_DATE(:date, 'DD-MM-YYYY')");
    query.bindValue(":date", date.toString("dd-MM-yyyy"));
    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur lors de la récupération des détails de la formation pour la date" << date << ":" << query.lastError().text();
        delete model;
        model = nullptr;
    }
    return model;
}

QSqlQueryModel* Formation::getTopRatedFormation() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM formation ORDER BY (NBR_LIKES - NBR_DISLIKES) DESC LIMIT 1");
    return model;
}

