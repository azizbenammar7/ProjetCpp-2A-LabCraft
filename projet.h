#ifndef PROJET_H
#define PROJET_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QPixmap> // Add QPixmap header for QR code storage
#include <QSqlQuery>
#include <QList> // Include QList for storing tasks
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QLegend>
#include <QGridLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPrinter>
#include <QPainter>
class projet
{
public:
    projet(){}
    projet(QString id_projet, QString nom, QString etat, QDate date_debut, QDate date_fin, int budget, QString id_employe, int id_client, QByteArray qrCode, QList<QString> taches);

    QString getNom() const { return nom; }
    QString getEtat() const { return etat; }
    QDate getDate_debut() const { return date_debut; }
    QDate getDate_fin() const { return date_fin; }
    QString getID() const { return id_projet; }
    QString getid_employe() const { return id_employe; }
    int getid_client() const { return id_client; }
    int getbudget() const { return budget; }
    QByteArray getQrCode() const { return qrCode; }
    QList<QString> getTaches() const { return taches; } // Getter for taches

    void setNom(QString n) { nom = n; }
    void setEtat(QString e) { etat = e; }
    void setDate_debut(QDate dd) { date_debut = dd; }
    void setDate_fin(QDate df) { date_fin = df; }
    void setid_employe(QString ide) { id_employe = ide; }
    void setID(QString id) { id_projet = id; }
    void setbudget(int b) { budget = b; }
    void setid_client(int ids) { id_client = ids; }
    void setQrCode(QByteArray code) { qrCode = code; }
    void setTaches(const QList<QString>& t) { taches = t; } // Setter for taches

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(QString);
    bool modifier(QString id_projet, QString nom, QString etat, QDate date_debut, QDate date_fin, int budget, QString id_employe,int id_client);
    QList<QString> getEmployeeIds();
    QList<int> getClientIds();
    static QList<projet> getProjects(); // Add static method declaration
    static QString getProjectIdFromName(const QString& projectName); // Declaration of the new static method
    bool updateTasksForProject(const QString& projectId, const QString& tacheFait, const QString& tacheEnCours, const QString& tacheAFaire);
    QList<QString> getTasksForProject(const QString& projectId);
    void createStat(QStackedWidget *stackedWidget);
    QSqlQueryModel* searchProjectByName(const QString& nomProjet);
    QSqlQueryModel* sortProjectsByBudget();
    void exportProjectInfoToPDF(int id_projet);
    bool fetchProjectDetailsForModification(const QString& id_projet, QString& nom, QString& etat, QDate& date_debut, QDate& date_fin, int& budget, QString& id_employe, int& id_client);

private:
    QString nom, etat, id_projet, id_employe;

    QDate date_debut, date_fin;
    int id_client, budget;
    QByteArray qrCode;
    QList<QString> taches; // New attribute to store taches
};

#endif // PROJET_H
