#include "employe.h"

employe::employe(int id_employe, int cin, QString nom, QString prenom, QString ddn, QString poste, QString mail, QString mdp, QString question, QString reponse, int salaire)
{
    this->id_employe = id_employe;
    this->nom = nom;
    this->prenom = prenom;
    this->ddn = ddn;
    this->poste = poste;
    this->mail = mail;
    this->mdp = mdp; // Initialisation du mot de passe
    this->question = question; // Initialisation de la question de sécurité
    this->reponse = reponse; // Initialisation de la réponse de sécurité
    this->cin = cin;
    this->salaire = salaire;
}

bool employe::ajouter_employe()
{
    QSqlQuery query;
    query.prepare("INSERT INTO employe (id_employe, nom, prenom, ddn, poste, mail, mdp, question, reponse, cin, salaire) "
                  "VALUES (:id_employe, :nom, :prenom, :ddn, :poste, :mail, :mdp, :question, :reponse, :cin, :salaire)");
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":ddn", ddn);
    query.bindValue(":poste", poste);
    query.bindValue(":mail", mail);
    query.bindValue(":mdp", mdp); // Ajout du mot de passe
    query.bindValue(":question", question); // Ajout de la question de sécurité
    query.bindValue(":reponse", reponse); // Ajout de la réponse de sécurité
    query.bindValue(":cin", cin);
    query.bindValue(":salaire", salaire);

    return query.exec();
}

QSqlQueryModel * employe::afficher_employe()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_employe, nom, prenom, ddn, poste, mail, mdp, question, reponse, cin, salaire FROM employe");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mot de passe"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Question de sécurité"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Réponse de sécurité"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Salaire"));
    return model;
}

bool employe::supprimer_employe(int id_employe)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);
    return query.exec();
}

bool employe::modifier_employe(int id_employe, int cin, QString nom, QString prenom, QString ddn, QString poste, QString mail, QString mdp, QString question, QString reponse, int salaire)
{
    QSqlQuery query;
    query.prepare("UPDATE employe SET nom = :nom, prenom = :prenom, ddn = :ddn, poste = :poste, mail = :mail, mdp = :mdp, question = :question, reponse = :reponse, cin = :cin, salaire = :salaire WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":ddn", ddn);
    query.bindValue(":poste", poste);
    query.bindValue(":mail", mail);
    query.bindValue(":mdp", mdp);
    query.bindValue(":question", question);
    query.bindValue(":reponse", reponse);
    query.bindValue(":cin", cin);
    query.bindValue(":salaire", salaire);
    return query.exec();
}

bool employe::verifierConnexion(QString email, QString password, QString &poste)
{
    QSqlQuery query;
    query.prepare("SELECT poste FROM employe WHERE mail = :email AND mdp = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        poste = query.value(0).toString();
        return true;
    } else {
        return false;
    }
}

bool employe::modifierMotDePasse(QString email, QString questionUtilisateur, QString reponseUtilisateur, QString nouveauMdp)
{
    QSqlQuery query;
    query.prepare("SELECT question, reponse FROM employe WHERE mail = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        QString question = query.value(0).toString();
        QString reponse = query.value(1).toString();

        if (questionUtilisateur == question && reponseUtilisateur == reponse) {
            query.prepare("UPDATE employe SET mdp = :mdp WHERE mail = :email");
            query.bindValue(":mdp", nouveauMdp);
            query.bindValue(":email", email);

            if (query.exec()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

QSqlQuery employe::obtenirRepartitionPostesQuery() {
    QSqlQuery query("SELECT poste, COUNT(*) FROM employe GROUP BY poste");
    return query;
}
QSqlQuery employe::obtenirStatistiquesPostesQuery() {
    QSqlQuery query("SELECT poste, COUNT(*) FROM employe GROUP BY poste");
    return query;
}
bool employe::verifierExistenceEmployeParID(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM employe WHERE id_employe = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0; // Si count > 0, alors l'employé existe, sinon il n'existe pas
    } else {
        // Erreur lors de l'exécution de la requête
        //qDebug() << "Erreur lors de la vérification de l'existence de l'employé par ID.";
        return false;
    }
}
