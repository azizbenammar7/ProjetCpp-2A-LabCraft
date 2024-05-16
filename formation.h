#ifndef FORMATION_H
#define FORMATION_H

#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>

class Formation
{
    QString titre, description, date_debut, niveau_formation;
    int id_formations, cout, id_employe, id_salle;
    int nbr_likes;
    int nbr_dislikes;

public:
    Formation(){}
    Formation(QString, QString, QString, QString, int, int, int, int, int);

    QString getTitre() const { return titre; }
    QString getDescription() const { return description; }
    QString getDateDebut() const { return date_debut; }
    QString getNiveauFormation() const { return niveau_formation; }
    int getIdFormations() const { return id_formations; }
    int getCout() const { return cout; }
    int getIdEmploye() const { return id_employe; }
    int getIdSalles() const { return id_salle; }
    int getNbrLikes() const { return nbr_likes; } // Accesseur pour nbr_likes
    int getNbrDislikes() const { return nbr_dislikes; } // Accesseur pour nbr_dislikes

    void setTitre(QString t) { titre = t; }
    void setDescription(QString d) { description = d; }
    void setDateDebut(QString dd) { date_debut = dd; }
    void setNiveauFormation(QString nf) { niveau_formation = nf; }
    void setIdFormations(int id) { id_formations = id; }
    void setCout(int c) { cout = c; }
    void setIdEmploye(int id) { id_employe = id; }
    void setIdSalles(int id) { id_salle = id; }
    void setNbrLikes(int likes) { nbr_likes = likes; } // Mutateur pour nbr_likes
    void setNbrDislikes(int dislikes) { nbr_dislikes = dislikes; } // Mutateur pour nbr_dislikes

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool modifier(int id_formations, QString titre, QString description, QString date_debut, QString niveau_formation, int cout, int id_employe, int id_salle, int nbr_likes, int nbr_dislikes);
    QList<int> getEmployeeIds();
    QList<int> getSalleIds();
    Formation getFormationInfo(int id);
    void exportToPDF(const Formation& formationInfo);
    QSqlQueryModel* filtrerFormationsNextWeek();
    QSqlQueryModel* getSortedFormationsByCout();
    QSqlQueryModel* searchFormationsByTitre(const QString& titre);
    QSqlQueryModel* getFormationsCountByMonth();

    QList<QDate> getFormationDates();
    QSqlQueryModel* getFormationDetailsByDate(const QDate& date);
    QList<QDate> getProchainesDatesFormations();

    QSqlQueryModel* getTopRatedFormation();
};

#endif // FORMATION_H
