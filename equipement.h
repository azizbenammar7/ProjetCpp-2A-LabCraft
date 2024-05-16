#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QByteArray> // Ajout pour le type blob
#include <QtCharts>


class equipement
{
    QString designation, categorie, description, etat;
    QString video_equipement; // Nouvel attribut pour la vidéo
    int id_equipement, cout;

public:
    equipement(){}
    equipement(int, QString, int, QString, QString, QString, QString); // Modifier le constructeur

    QString getDesignation() const { return designation; }
    QString getEtat() const { return etat; }
    QString getCategorie() const { return categorie; }
    QString getDescription() const { return description; }
    int getID() const { return id_equipement; }
    int getCout() const { return cout; }
    QString getVideoEquipement() const { return video_equipement; }

    void setDesignation(QString n) { designation = n; }
    void setEtat(QString e) { etat = e; }
    void setCategorie(QString dd) { categorie = dd; }
    void setDescription(QString df) { description = df; }
    void setID(int id) { id_equipement = id; }
    void setCout(int b) { cout = b; }
    void setVideoEquipement(QString video) { video_equipement = video; }

    bool ajouterequipement();
    QSqlQueryModel * afficherequipement();
    bool supprimerequipement(int);
    bool modifierequipement(int id_equipement, QString designation, int cout, QString categorie, QString description, QString etat, QString video_equipement); // Modifier la méthode de modification
    bool exporterEquipementPDF(int id_equipement);
    QSqlQueryModel *trierEquipementParCout();
    QSqlQueryModel* chercherEquipementParDesignation(const QString &nomEquipement);
    QChartView* genererDiagrammeStatistiqueParCategorie();
    QChartView* genererDiagrammeRepartitionEquipementsParCategorie();
    bool enregistrerNoteEquipement(int id_equipement, int note);
    QString obtenirCheminVideoEquipement(int id_equipement);





};

#endif // EQUIPEMENT_H
