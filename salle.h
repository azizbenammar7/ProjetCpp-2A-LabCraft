#ifndef SALLES_H
#define SALLES_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Salle
{
    int id_salle;
    int capacite;
    double prix_jour;
    QString etat;
    int modifications;
    int temp_max;

public:
    Salle() : modifications(0) {}
    Salle(int id_salle, int capacite, double prix_jour, QString etat,double temp_max);

    int getIdSalle() const { return id_salle; }
    int getCapacite() const { return capacite; }
    double getPrixJour() const { return prix_jour; }
    QString getEtat() const { return etat; }
    double getTemp_max() const { return temp_max; }
    int getModifications() const { return modifications; }

    void setIdSalle(int id) {
        id_salle = id;
    }
    void setCapacite(int cap) { capacite = cap; }
    void setPrixJour(double prix) { prix_jour = prix; }
    void setEtat(const QString& et) { etat = et; }
    void setTemp_max(double tmp) { temp_max = tmp; }

    bool ajouter();

    //Salle(int id_salle, int capacite, double prix_jour, QString etat,double temp_max);

    QSqlQueryModel* afficher();
    bool supprimer(int id_salle);
    bool modifier(int id_salle, int capacite, double prix_jour, const QString &etat,double temp_max);
    void getSalleEtatFromDatabase(int id_salle, QString &etat);

    void incrementModifications();
    static Salle getSalleFromDatabase(int id_salle);
    QSqlQueryModel* trierSalleParPrix();
    QSqlQueryModel* rechercherSallesParCapacite(int capaciteMin);
    QSqlQueryModel* exporterSalleInfoToPDF(int id_salle) ;
    QSqlQueryModel* getSalleStatsByEtat();
    QSqlQueryModel* getSalleStatByEtat();
    QList<Salle> getSallesFromDatabase();





};

#endif
