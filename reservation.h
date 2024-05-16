#ifndef RESERVATION_H
#define RESERVATION_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QDate>






class reservation
{
    QString  heure ,id_projet, id_equipement;
    QDate dates ;
public:
    reservation(){}
    reservation(QString,QString,QDate,QString);


    QDate getdate(){return dates;}
    QString getid_equipement(){return id_equipement;}
    QString getid_projet(){return id_projet;}
    QString getheure(){return heure;}




    void setid_equipement(QString n){id_equipement=n;}
    void setdate(QDate dd){dates=dd;}
    void setid_projet(QString ide){ id_projet=ide;}
    void setheure(QString ids){ heure=ids;}


    bool ajouter();
    QList<QString> getprojetIds();
    QList<QString> getequipementIds();
    QSqlQueryModel * afficherreservation();

    bool updateEquipmentState(const QString &state, QString &error);

};

#endif // RESERVATION_H
