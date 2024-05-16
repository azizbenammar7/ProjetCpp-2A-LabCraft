
#include "secdialog.h"
#include "ui_secdialog.h"
#include<projet.h>
#include<salle.h>
#include<arduino.h>
#include<equipement.h>
#include<mainwindow.h>
#include<formation.h>
#include<client.h>
#include<smtp.h>
#include<salle.h>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QDir>
#include <QtCharts>
#include <QLineSeries>
#include <QScatterSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QPieSeries>
#include <QPieSlice>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QProcess>
#include <QCoreApplication>
#include <QTranslator>
#include <QChartView>
#include <QBarCategoryAxis>
#include <QDateTime>
#include <QSqlError>
#include <QtWidgets>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickView>
#include <QSqlRecord>
#include <QDesktopServices>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>


SecDialog::SecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);

    ui->tableView_3->setModel(emp.afficher_employe());
    ui->tableView_equipement->setModel(equi.afficherequipement());
    ui->tableView->setModel(client.afficher());
    ui->tablesalle->setModel(salles.afficher());
    ui->tableView_4->setModel(formation.afficher());
    ui->tableView_projet->setModel(proj.afficher());

    ui->tableView_reservation->setModel(reserve.afficherreservation());
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
    reservation R;
    projet P;


    ui->comboBoxIDemployefk->clear();
    ui->comboBoxIDsallefk->clear();
    ui->comboBoxIDemployefk_2->clear();
    ui->comboBoxIDsallefk_2->clear();
    ui->comboBox_IDclientfk->clear(); // Clear existing items first
    ui->comboBox_IDclientfk_2->clear(); // Clear existing items first
    ui->comboBox_IDprojets->clear(); // Clear existing items first
    ui->comboBox_IDequipements->clear(); // Clear existing items first
    Formation F;
    QList<int> employeeIds = F.getEmployeeIds();
    QList<int> sallesIds = F.getSalleIds();
    QList<QString> employeeIdss = P.getEmployeeIds();
    QList<int> clientIds = P.getClientIds();

    for (const int& id : employeeIds)
    {
        ui->comboBoxIDemployefk->addItem(QString::number(id));
        ui->comboBoxIDemployefk_2->addItem(QString::number(id));
    }

    for (const int& id : sallesIds)
    {
        ui->comboBoxIDsallefk->addItem(QString::number(id));
        ui->comboBoxIDsallefk_2->addItem(QString::number(id));
    }
           for (const int & id : employeeIds) {
               ui->comboBox_IDemployefk->addItem(QString::number(id));
               ui->comboBox_IDemployefk_2->addItem(QString::number(id));

           }
           for (const int& id : clientIds) {
               ui->comboBox_IDclientfk->addItem(QString::number(id));
               ui->comboBox_IDclientfk_2->addItem(QString::number(id));

           }
           QList<QString> projetIds=R.getprojetIds() ;
           for (const QString& id : projetIds) {
               ui->comboBox_IDprojets->addItem(id);

           }
           QList<QString> equipementIds=R.getequipementIds() ;
           for (const QString& id : equipementIds) {
               ui->comboBox_IDequipements->addItem(id);

           }




    //ui->tableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    connect(ui->pushButton_selectImage, &QPushButton::clicked, this, &SecDialog::on_pushButton_selectImage_clicked);
    connect(A.getserial(), &QSerialPort::readyRead, this, &SecDialog::onSerialDataReceived);



}
void SecDialog::setCurrentPage(int index) {
    ui->stackedWidget->setCurrentIndex(index);
}
SecDialog::~SecDialog()
{
    delete ui;
}
void SecDialog::on_pushButton_144_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->listeclient);
}
void SecDialog::on_pushButton_237_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->listesalle);
}
void SecDialog::on_pushButton_238_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->ajoutsalle);
}

void SecDialog::on_pushButton_239_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->statsalle);
}


void SecDialog::on_pushButton_145_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutclient);

}

void SecDialog::on_pushButton_146_clicked()
{

    ui->stackedWidget->setCurrentWidget(ui->statclient);

      // Récupérer les données de statistiques sur les clients par sexe
      QSqlQuery query;
      query.exec("SELECT email, COUNT(*) FROM Clients GROUP BY email ORDER BY email"); // Sélectionner les clients par sexe

      // Créer un modèle de données pour le sexe
      QBarSeries *series = new QBarSeries();

      while (query.next()) {
          QString email = query.value(0).toString();
          int count = query.value(1).toInt();

          QBarSet *set = new QBarSet(email);
          *set << count; // Ajouter le nombre de clients de chaque sexe
          series->append(set);
      }

      // Créer un graphique
      QChart *chart = new QChart();
      chart->setTitle("Statistiques des clients par adresse");
      chart->addSeries(series);
      chart->setAnimationOptions(QChart::SeriesAnimations);

      // Créer des axes
      QBarCategoryAxis *axisX = new QBarCategoryAxis();
      QValueAxis *axisY = new QValueAxis();

      chart->addAxis(axisX, Qt::AlignBottom);
      chart->addAxis(axisY, Qt::AlignLeft);

      series->attachAxis(axisX);
      series->attachAxis(axisY);

      // Créer une vue graphique pour le graphique
      QChartView *chartView = new QChartView(chart);
      chartView->setRenderHint(QPainter::Antialiasing);

      // Ajuster les plages des axes
      axisY->setRange(0, query.size()); // Supposition: ajuster selon le nombre maximum de clients d'un sexe
      axisY->setTickCount(10); // Ajuster selon le besoin

      // Afficher le graphique
      ui->stackedWidget->addWidget(chartView);
      ui->stackedWidget->setCurrentWidget(chartView);


}

void SecDialog::on_pushButton_84_clicked()
{

    if (posteEmployeConnecte == "Responsable employe" || posteEmployeConnecte == "admin") {
        ui->stackedWidget->setCurrentWidget(ui->employe);
        } else {
            ui->label_message_menu->setText("accès interdit");
        }

}

void SecDialog::on_pushButton_85_clicked()
{

    if (posteEmployeConnecte == "Responsable équipements" || posteEmployeConnecte == "admin") {
        ui->stackedWidget->setCurrentWidget(ui->equipement);
        } else {
            ui->label_message_menu->setText("accès interdit");
        }

}

void SecDialog::on_pushButton_86_clicked()
{

    if (posteEmployeConnecte == "Responsable projets" || posteEmployeConnecte == "admin") {
        ui->stackedWidget->setCurrentWidget(ui->projet);
        } else {
            ui->label_message_menu->setText("accès interdit");
        }

}

void SecDialog::on_pushButton_87_clicked()
{

    if (posteEmployeConnecte == "Responsable client" || posteEmployeConnecte == "admin") {
        ui->stackedWidget->setCurrentWidget(ui->client);
        } else {
            ui->label_message_menu->setText("accès interdit");
        }

}

void SecDialog::on_pushButton_93_clicked()
{

    if (posteEmployeConnecte == "Responsable salles" || posteEmployeConnecte == "admin") {
        ui->stackedWidget->setCurrentWidget(ui->salle);
        } else {
            ui->label_message_menu->setText("accès interdit");
        }

}

void SecDialog::on_pushButton_88_clicked()
{

    if (posteEmployeConnecte == "Responsable formation" || posteEmployeConnecte == "admin") {
        ui->stackedWidget->setCurrentWidget(ui->formation);
        } else {
            ui->label_message_menu->setText("accès interdit");
        }

}

void SecDialog::on_pushButton_147_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeprojet);

}
void SecDialog::on_pushButton_148_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutprojet);

}

void SecDialog::on_pushButton_150_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeemploye);

}
void SecDialog::on_pushButton_151_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutemploye);

}
void SecDialog::on_pushButton_152_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statemploye);

}

void SecDialog::on_pushButton_153_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeequipement);

}
void SecDialog::on_pushButton_154_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutequipement);

}
void SecDialog::on_pushButton_155_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statequipement);

}

void SecDialog::on_pushButton_156_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeformation);

}
void SecDialog::on_pushButton_157_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutformation);

}
void SecDialog::on_pushButton_158_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statformation);

}

void SecDialog::on_pushButton_159_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listesalle);

}
void SecDialog::on_pushButton_160_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutsalle);

}
void SecDialog::on_pushButton_161_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->statsalle);

    Salle salle;
    QSqlQueryModel* model = salle.getSalleStatsByEtat();

    if (model) {
        QBarSet *reserveSet = new QBarSet("Salles Réservées");
        QBarSet *nonReserveSet = new QBarSet("Salles Non Réservées");

        for (int i = 0; i < model->rowCount(); ++i) {
            QString etat = model->record(i).value(0).toString().toLower();
            int count = model->record(i).value(1).toInt();
            if (etat == "reserve") {
                *reserveSet << count;
            } else if (etat == "non reserve") {
                *nonReserveSet << count;
            }
        }

        QBarSeries *series = new QBarSeries();
        series->append(reserveSet);
        series->append(nonReserveSet);

        QChart *chart = new QChart();
        chart->setTitle("Statistiques de salles par état");
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        QValueAxis *axisY = new QValueAxis();

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        series->attachAxis(axisX);
        series->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        axisX->setRange("Réservées", "Non réservées");
        axisY->setRange(0, 10);
        axisY->setTickCount(11);

        ui->stackedWidget->addWidget(chartView);
        ui->stackedWidget->setCurrentWidget(chartView);
    } else {
        qDebug() << "Erreur lors de la récupération des statistiques de salle par état.";
    }
}

void SecDialog::on_pushButton_97_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutprojet);

}



void SecDialog::on_pushButton_92_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeprojet);

}



void SecDialog::on_pushButton_90_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutprojet);

}

void SecDialog::on_pushButton_89_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeprojet);

}

void SecDialog::on_pushButton_142_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutclient);

}

void SecDialog::on_pushButton_143_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statclient);

      // Récupérer les données de statistiques sur les clients par sexe
      QSqlQuery query;
      query.exec("SELECT email, COUNT(*) FROM Clients GROUP BY email ORDER BY email"); // Sélectionner les clients par sexe

      // Créer un modèle de données pour le sexe
      QBarSeries *series = new QBarSeries();

      while (query.next()) {
          QString email = query.value(0).toString();
          int count = query.value(1).toInt();

          QBarSet *set = new QBarSet(email);
          *set << count; // Ajouter le nombre de clients de chaque sexe
          series->append(set);
      }

      // Créer un graphique
      QChart *chart = new QChart();
      chart->setTitle("Statistiques des clients par adresse");
      chart->addSeries(series);
      chart->setAnimationOptions(QChart::SeriesAnimations);

      // Créer des axes
      QBarCategoryAxis *axisX = new QBarCategoryAxis();
      QValueAxis *axisY = new QValueAxis();

      chart->addAxis(axisX, Qt::AlignBottom);
      chart->addAxis(axisY, Qt::AlignLeft);

      series->attachAxis(axisX);
      series->attachAxis(axisY);

      // Créer une vue graphique pour le graphique
      QChartView *chartView = new QChartView(chart);
      chartView->setRenderHint(QPainter::Antialiasing);

      // Ajuster les plages des axes
      axisY->setRange(0, query.size()); // Supposition: ajuster selon le nombre maximum de clients d'un sexe
      axisY->setTickCount(10); // Ajuster selon le besoin

      // Afficher le graphique
      ui->stackedWidget->addWidget(chartView);
      ui->stackedWidget->setCurrentWidget(chartView);


}

void SecDialog::on_pushButton_138_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeclient);

}

void SecDialog::on_pushButton_140_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statclient);

}

void SecDialog::on_pushButton_136_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutclient);

}

void SecDialog::on_pushButton_135_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeclient);

}

void SecDialog::on_pushButton_134_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statsalle);

    Salle salle;
    QSqlQueryModel *model = salle.getSalleStatsByEtat();

    QPieSeries *series = new QPieSeries();
    for (int i = 0; i < model->rowCount(); ++i) {
        QString etat = model->record(i).value(0).toString();
        int count = model->record(i).value(1).toInt();
        series->append(etat, count);
    }

    QChart *chart = new QChart();
    chart->setTitle("Statistiques de salles par état");

    chart->addSeries(series);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);
}

void SecDialog::on_pushButton_133_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutsalle);

}

void SecDialog::on_pushButton_129_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listesalle);

}

void SecDialog::on_pushButton_131_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statsalle);

    Salle salle;
    QSqlQueryModel *model = salle.getSalleStatsByEtat();

    QPieSeries *series = new QPieSeries();
    for (int i = 0; i < model->rowCount(); ++i) {
        QString etat = model->record(i).value(0).toString();
        int count = model->record(i).value(1).toInt();
        series->append(etat, count);
    }

    QChart *chart = new QChart();
    chart->setTitle("Statistiques de salles par état");

    chart->addSeries(series);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);

}

void SecDialog::on_pushButton_127_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutsalle);

}

void SecDialog::on_pushButton_126_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listesalle);

}

void SecDialog::on_pushButton_124_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutemploye);

}

void SecDialog::on_pushButton_125_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statemploye);

}

void SecDialog::on_pushButton_120_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeemploye);

}

void SecDialog::on_pushButton_122_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statemploye);

}

void SecDialog::on_pushButton_117_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeemploye);

}

void SecDialog::on_pushButton_118_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutemploye);

}

void SecDialog::on_pushButton_115_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutequipement);

}

void SecDialog::on_pushButton_116_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statequipement);

}

void SecDialog::on_pushButton_111_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeequipement);

}

void SecDialog::on_pushButton_113_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statequipement);

}

void SecDialog::on_pushButton_109_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutequipement);

}

void SecDialog::on_pushButton_108_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeequipement);

}

void SecDialog::on_pushButton_106_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutformation);

}

void SecDialog::on_pushButton_107_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statformation);

}

void SecDialog::on_pushButton_102_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeformation);

}

void SecDialog::on_pushButton_104_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statformation);

}

void SecDialog::on_pushButton_100_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ajoutformation);

}

void SecDialog::on_pushButton_99_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeformation);

}













//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SecDialog::on_pushButton_annulerajou_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeemploye);

}

void SecDialog::on_pushButton_ajouteremploye_2_clicked()
{
    // Récupération des valeurs depuis l'interface utilisateur
    QString nom = ui->lineEdit_nom_4->text();
    QString prenom = ui->lineEdit_prenom_3->text();
    int id_employe = ui->lineEdit_idemploye_3->text().toInt();
    QString ddn = ui->dateEdit_employe_3->date().toString("yyyy-MM-dd");
    QString cinString = ui->lineEdit_cin_3->text();
    QString poste = ui->comboBox_poste_3->currentText();
    QString mail = ui->lineEdit_mail_3->text();
    QString mdp = ui->lineEdit_mdp_2->text(); // Obtention du mot de passe
    QString question = ui->comboBox_question_2->currentText(); // Obtention de la question
    QString reponse = ui->lineEdit_reponse_2->text(); // Obtention de la réponse
    int salaire = ui->doubleSpinBox_salaire_3->value();

    // Expression régulière pour les lettres seulement
    QRegularExpression lettersOnlyRegex("^[a-zA-ZéèêëàáâäåçñÉÈÊËÀÁÂÄÅÇÑ]+$");

    // Expression régulière pour les chiffres seulement
    QRegularExpression digitsOnlyRegex("^[0-9]+$");

    // Expression régulière pour le format d'un e-mail simple (ne prend pas en compte toutes les possibilités)
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    // Vérification des contrôles de saisie
    if (!nom.contains(lettersOnlyRegex)) {
        qDebug() << "Erreur : Le nom doit contenir des lettres seulement.";
        return;
    }

    if (!prenom.contains(lettersOnlyRegex)) {
        qDebug() << "Erreur : Le prénom doit contenir des lettres seulement.";
        return;
    }

    if (!cinString.contains(digitsOnlyRegex)) {
        qDebug() << "Erreur : Le CIN doit contenir des chiffres seulement.";
        return;
    }

    if (!mail.contains(emailRegex)) {
        qDebug() << "Erreur : Le format de l'e-mail n'est pas valide.";
        return;
    }

    // Réinitialiser le message s'il n'y a pas d'erreur
    ui->label_message->clear();

    int cin = cinString.toInt();

    // Création de l'objet employé
    employe E(id_employe, cin, nom, prenom, ddn, poste, mail, mdp, question, reponse, salaire);

    // Modification de l'employé
    bool test = E.modifier_employe(id_employe, cin, nom, prenom, ddn, poste, mail, mdp, question, reponse, salaire);

    if (test) {
        qDebug() << "L'employé a été modifié avec succès dans la base de données.";
    } else {
        qDebug() << "Erreur lors de la modification de l'employé dans la base de données.";
    }

    // Mise à jour de la vue
    ui->tableView_3->setModel(E.afficher_employe());
    ui->stackedWidget->setCurrentWidget(ui->listeemploye);
}


void SecDialog::on_pushButton_ajouteremploye_clicked()
{
    QString nom = ui->lineEdit_nom_2->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString ddn = ui->dateEdit_employe->date().toString("yyyy-MM-dd");
    QString cinString = ui->lineEdit_cin->text();
    QString poste = ui->comboBox_poste->currentText();
    QString mail = ui->lineEdit_mail->text();
    int salaire = ui->doubleSpinBox_salaire->value();

    QString mdp = ui->lineEdit_mdp->text(); // Récupération du mot de passe
    QString question = ui->comboBox_question->currentText(); // Récupération de la question de sécurité
    QString reponse = ui->lineEdit_reponse->text(); // Récupération de la réponse de sécurité

    // Expressions régulières pour la validation des saisies
    QRegularExpression lettersOnlyRegex("^[a-zA-ZéèêëàáâäåçñÉÈÊËÀÁÂÄÅÇÑ]+$");
    QRegularExpression digitsOnlyRegex("^[0-9]+$");
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    // Vérification des contrôles de saisie
    if (!nom.contains(lettersOnlyRegex)) {
        ui->label_message->setText("Erreur : Le nom doit contenir des lettres seulement.");
        return;
    }

    if (!prenom.contains(lettersOnlyRegex)) {
        ui->label_message->setText("Erreur : Le prénom doit contenir des lettres seulement.");
        return;
    }

    if (!cinString.contains(digitsOnlyRegex)) {
        ui->label_message->setText("Erreur : Le CIN doit contenir des chiffres seulement.");
        return;
    }

    if (!mail.contains(emailRegex)) {
        ui->label_message->setText("Erreur : Le format de l'e-mail n'est pas valide.");
        return;
    }

    // Conversion du CIN en entier
    int cin = cinString.toInt();

    // Création de l'objet employé
    employe E(0, cin, nom, prenom, ddn, poste, mail, mdp, question, reponse, salaire);

    // Ajout de l'employé
    bool test = E.ajouter_employe();

    // Affichage du résultat de l'ajout
    if (test) {
        QMessageBox::information(this, "Succès", "L'employé a été ajouté avec succès à la base de données.");
        ui->stackedWidget->setCurrentWidget(ui->listeemploye);
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'employé à la base de données.");
    }

    // Réinitialiser le message s'il n'y a pas d'erreur
    ui->label_message->clear();

    // Mise à jour de la vue
    ui->tableView_3->setModel(E.afficher_employe());
    ui->stackedWidget->setCurrentWidget(ui->ajoutemploye);
}


void SecDialog::on_pushButton_modifieremploye_clicked()
{
    int id_employe = ui->lineEdit_8->text().toInt();


    QSqlQuery query;
    query.prepare("SELECT cin, nom, prenom, ddn, poste, mail, mdp, question, reponse, salaire FROM employe WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);

    if (query.exec() && query.next()) {
        int cin = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString ddn = query.value(3).toString();
        QString poste = query.value(4).toString();
        QString mail = query.value(5).toString();
        QString mdp = query.value(6).toString();
        QString question = query.value(7).toString();
        QString reponse = query.value(8).toString();
        int salaire = query.value(9).toInt();

        // Populate the input fields with the fetched details
        ui->lineEdit_cin_3->setText(QString::number(cin));
        ui->lineEdit_nom_4->setText(nom);
        ui->lineEdit_prenom_3->setText(prenom);
        ui->lineEdit_idemploye_3->setText(QString::number(id_employe));

        // Utilisation de dateEdit_employe2 pour afficher la date de naissance
        QDate date = QDate::fromString(ddn, "yyyy-MM-dd");
        ui->dateEdit_employe_3->setDate(date);
        ui->lineEdit_mail_3->setText(mail);

        // Utilisation de comboBox_poste2 pour afficher le poste
        int indexPoste = ui->comboBox_poste_3->findText(poste);
        if (indexPoste != -1) {
            ui->comboBox_poste_3->setCurrentIndex(indexPoste);
        }

        // Utilisation de doubleSpinBox_salaire2 pour afficher le salaire
        ui->doubleSpinBox_salaire_3->setValue(salaire);

        // Utilisation de lineEdit_mdp2 pour afficher le mot de passe
        ui->lineEdit_mdp_2->setText(mdp);


        int indexQuestion = ui->comboBox_question_2->findText(question);
        if (indexQuestion != -1) {
            ui->comboBox_question_2->setCurrentIndex(indexQuestion);
        }


        ui->lineEdit_reponse_2->setText(reponse);


        ui->stackedWidget->setCurrentWidget(ui->modifieremploye);
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Failed to fetch employee details for modification."),
                              QMessageBox::Ok);
    }
}


void SecDialog::on_pushButton_supprimeremploye_clicked()
{
    int id_employe = ui->lineEdit_8->text().toInt();
    bool test = emp.supprimer_employe(id_employe);

    if (test)
    {

        ui->tableView_3->setModel(emp.afficher_employe());
        QMessageBox::information(this, tr("Succès"),
                                 tr("Suppression effectuée.\n"
                                    "Cliquez sur Annuler pour quitter."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Suppression non effectuée.\n"
                                 "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
    }
}

void SecDialog::on_pushButton_trier_clicked()
{

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_employe, nom, prenom, ddn, poste, mail, cin, salaire, mdp, question, reponse FROM employe ORDER BY nom");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Mot de passe"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Question"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Réponse"));


    ui->tableView_3->setModel(model);
}

void SecDialog::on_pushButton_chercheremploye_clicked()
{

    int idEmploye = ui->lineEdit_8->text().toInt();


    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id_employe, nom, prenom, ddn, poste, mail, cin, salaire, mdp, question, reponse FROM employe WHERE id_employe = :idEmploye");
    query.bindValue(":idEmploye", idEmploye);
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête de recherche.";
        return;
    }


    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Mot de passe"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Question"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Réponse"));


    ui->tableView_3->setModel(model);
}

void SecDialog::on_pushButton_35_clicked()
{

    int id_employe = ui->lineEdit_8->text().toInt();


    QSqlQuery query;
    query.prepare("SELECT nom, prenom, ddn, poste, mail, cin, salaire, mdp, question, reponse FROM employe WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des informations de l'employé pour l'exportation.";
        return;
    }

    if (!query.next()) {
        qDebug() << "Aucun employé trouvé avec l'ID spécifié.";
        return;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QString ddn = query.value(2).toString();
    QString poste = query.value(3).toString();
    QString mail = query.value(4).toString();
    int cin = query.value(5).toInt();
    int salaire = query.value(6).toInt();
    QString mdp = query.value(7).toString(); // Nouvelle colonne
    QString question = query.value(8).toString(); // Nouvelle colonne
    QString reponse = query.value(9).toString(); // Nouvelle colonne


    QString fileName = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/employepdf/" + QString::number(id_employe) + "_employe_info.pdf";


    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);


    QPainter painter;
    painter.begin(&printer);


    painter.drawText(100, 100, "Nom: " + nom);
    painter.drawText(100, 120, "Prénom: " + prenom);
    painter.drawText(100, 140, "Date de naissance: " + ddn);
    painter.drawText(100, 160, "Poste: " + poste);
    painter.drawText(100, 180, "Mail: " + mail);
    painter.drawText(100, 200, "CIN: " + QString::number(cin));
    painter.drawText(100, 220, "Salaire: " + QString::number(salaire));
    painter.drawText(100, 240, "Mot de passe: " + mdp); // Nouvelle colonne
    painter.drawText(100, 260, "Question: " + question); // Nouvelle colonne
    painter.drawText(100, 280, "Réponse: " + reponse); // Nouvelle colonne

    // Fin du dessin
    painter.end();

    qDebug() << "Informations de l'employé exportées avec succès vers le fichier PDF: " << fileName;
}


void SecDialog::on_pushButton_diagramme_clicked() {
    QSqlQuery query = employe::obtenirStatistiquesPostesQuery();
    QBarSeries *series = new QBarSeries();

    while (query.next()) {
        QString poste = query.value(0).toString();
        int count = query.value(1).toInt();
        QBarSet *set = new QBarSet(poste);
        *set << count;
        series->append(set);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques d'employés par poste");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);
}

void SecDialog::on_pushButton_diagramme_2_clicked() {
    QSqlQuery query = employe::obtenirRepartitionPostesQuery();
    QPieSeries *pieSeries = new QPieSeries();
    int totalEmployes = 0;

    while (query.next()) {
        QString poste = query.value(0).toString();
        int count = query.value(1).toInt();
        pieSeries->append(poste, count);
        totalEmployes += count;
    }

    for (QPieSlice *slice : pieSeries->slices()) {
        slice->setLabel(QString("%1 (%2%)").arg(slice->label()).arg(slice->percentage() / totalEmployes * 100));
    }

    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Répartition des employés par poste (Secteurs empilés)");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);
}

void SecDialog::setPosteEmployeConnecte(const QString &poste) {
    posteEmployeConnecte = poste;
}

void SecDialog::on_pushButton_annulermodif_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->listeemploye);
}


void SecDialog::on_pushButton_logout_clicked()
{

           MainWindow *mainWindow = qobject_cast<MainWindow*>(parentWidget());
           if (mainWindow)
           {
               hide(); // Cacher
               mainWindow->show();
           }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void SecDialog::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_10->text().toInt();
    Client client;
    bool test = client.supprimer(id);
    ui->tableView->setModel(client.afficher());



}
void SecDialog::on_pushButton_modifier_client_clicked()
{
    // Récupérer l'ID du client depuis l'interface graphique
        int id = ui->lineEdit_10->text().toInt(); // Assume this is the lineEdit for ID input

        // Vérifier si l'ID du client existe dans la base de données
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT NOM, PRENOM, SEXE,EMAIL, ADRESSE,DATE_DE_NAISSANCE, DATE_DINSCRIPTION, IMAGE FROM CLIENTS WHERE ID = :id");
        checkQuery.bindValue(":id", id);


        if (checkQuery.exec() && checkQuery.next()) {
            // L'ID existe, extraire les valeurs de la requête
            QString nom = checkQuery.value(0).toString();
            QString prenom = checkQuery.value(1).toString();
            QString sexe = checkQuery.value(2).toString();
            QString email = checkQuery.value(4).toString();
            QString adresse = checkQuery.value(3).toString();
            QString dateNaissance = checkQuery.value(5).toString();
            QString dateInscription = checkQuery.value(6).toString();
            QByteArray image = checkQuery.value(7).toByteArray();

            QDate dateDeNaissanceDate = QDate::fromString(dateNaissance, "yyyy-MM-dd");
            QDate dateInscriptionDate = QDate::fromString(dateInscription, "yyyy-MM-dd");


            // Mettre à jour les champs de l'interface graphique avec les valeurs précédentes
            ui->lineEdit_sexe_up->setText(sexe);
            ui->lineEdit_nom_up->setText(nom);
            ui->lineEdit_prenom_up->setText(prenom);
            ui->dateEdit_DDN_up->setDate(dateDeNaissanceDate);
            ui->lineEdit_adresse_up->setText(adresse);
            ui->lineEdit_email_up->setText(email);
            ui->dateEdit_inscrit_up->setDate(dateInscriptionDate);

            // Rendre le champ ID en lecture seule pour éviter des modifications
            ui->lineEdit_id_up->setReadOnly(true); // Assumer que c'est le champ pour l'ID

            // Changer vers la page de modification pour un client
            // Assumer que vous avez un widget empilé pour la navigation, et "modifClient" est la page de modification du client
            ui->stackedWidget->setCurrentWidget(ui->modifClient);
        } else {
            // L'ID n'existe pas, afficher un avertissement
            QMessageBox::warning(nullptr, "Erreur", "ID de client non trouvé dans la base de données !");
}
}
void SecDialog::on_pushButton_exportClient_clicked()
{
    // Récupérer l'ID de l'équipement à exporter
    int id = ui->lineEdit_10->text().toInt();

    // Récupérer les informations de l'équipement depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT  nom, prenom, sexe, email, adresse, date_de_naissance, date_dinscription FROM CLIENTS WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des informations de l'équipement pour l'exportation.";
        return;
    }

    if (!query.next()) {
        qDebug() << "Aucun équipement trouvé avec l'ID spécifié.";
        return;
    }
    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QString email = query.value(3).toString();

    QString sexe = query.value(2).toString();
    QString adresse = query.value(4).toString();
    QString dateNaissance = query.value(5).toString();
       QString dateInscription = query.value(6).toString();
    // Définir le nom du fichier PDF
    QString fileName = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichierclient/client_info_" + QString::number(id) + ".pdf";

    // Créer le périphérique d'impression
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Créer un painter associé au périphérique d'impression
    QPainter painter;
    painter.begin(&printer);

    // Dessiner les informations de l'équipement sur le painter
    painter.drawText(100, 100, "sexe: " + sexe);
    painter.drawText(100, 120, "nom: " + nom);
    painter.drawText(100, 140, "prenom: " + prenom);
    painter.drawText(100, 160, "adresse: " + adresse);
    painter.drawText(100, 180, "email: " + email);
    painter.drawText(100, 200, "dateNaissance: " + dateNaissance);
    painter.drawText(100, 220, "dateInscription: " + dateInscription);


    // Fin du dessin
    painter.end();

    qDebug() << "Informations de l'équipement exportées avec succès vers le fichier PDF: " << fileName;
}



void SecDialog::on_pushButton_selectImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choisir une image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            m_imageData = file.readAll();  // Stocker les données d'image

            // Afficher l'image
            QPixmap pixmap;
            pixmap.loadFromData(m_imageData);
            ui->label_imagePreview->setPixmap(pixmap.scaled(ui->label_imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier image."));
        }
    } else {
        QMessageBox::warning(this, tr("Aucune image sélectionnée"), tr("Aucune image n'a été sélectionnée."));
    }
}
/*
void SecDialog::on_pushButton_selectImage_up_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Loader"), tr("Cannot load %1.").arg(fileName));
            return;
        }

        ui->label_imagePreview->setPixmap(QPixmap::fromImage(image).scaled(ui->label_imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Convertir l'image en QByteArray pour stockage dans la BD
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG"); // sauvegarde l'image dans le format PNG dans le buffer

        // Stocker le QByteArray dans une variable membre pour l'utiliser lors de la mise à jour
        mCurrentImageData = imageData;
    }
}
*/
void SecDialog::on_pushButton_trier_client_clicked() {
    Client client;
    QSqlQueryModel* model = client.trierClientsParDateInscription();
    if (!model) {
        qDebug() << "Erreur lors de la mise à jour du modèle de vue.";
        return;
    }
    ui->tableView->setModel(model);
}
void SecDialog::on_pushButton_chercher_client_clicked() {
    // Récupérer le nom à rechercher depuis l'interface utilisateur
       QString nom = ui->lineEdit_search_cl->text();

       // Appeler la fonction pour rechercher les clients par nom
       Client client;
       QSqlQueryModel* searchModel = client.chercherClient(nom);

       // Vérifier si le modèle de recherche est valide
       if (!searchModel)
           return;

       // Mettre à jour la vue avec le modèle de recherche
       ui->tableView->setModel(searchModel);
}

void SecDialog::on_pushButton_ajouter_2_clicked()
{

        QString nom = ui->lineEdit_nom_3->text().trimmed();
        QString prenom = ui->lineEdit_prenom_2->text().trimmed();
        // int id = ui->lineEdit_ID->text().toInt(); // Commenté si l'ID n'est pas utilisé ici
        QString sexe = ui->lineEdit_sexe->text().trimmed();
        QString adresse = ui->lineEdit_adresse->text().trimmed();
        QString email = ui->lineEdit_email->text().trimmed();

        QString dateNaissance = ui->dateEdit_DDN->text();
        QString dateInscription = ui->dateEdit_inscrit->text();

       // Contrôles de saisie
        if(nom.isEmpty() || prenom.isEmpty() || sexe.isEmpty() || adresse.isEmpty() )
        {
            QMessageBox::warning(nullptr, tr("Champs manquants"), tr("Veuillez remplir tous les champs."));
            // Arrête l'exécution si des champs requis sont vides
            return;
        }
        if (m_imageData.isEmpty()) {
              QMessageBox::warning(this, tr("Aucune image sélectionnée"), tr("Veuillez sélectionner une image avant de continuer."));
              return;
          }

          // Utilisez m_imageData pour créer l'objet Client
          Client client(0, sexe, nom, prenom, adresse, email, dateNaissance, dateInscription, m_imageData);
        bool test = client.ajouter();

        if (test)
           {
               ui->tableView->setModel(client.afficher());

               // Send Welcome Email
               Smtp* smtp = new Smtp("tassounaassoula@gmail.com", "awhc rtpj ujnd binw", "gmail.example.com", 587);
               connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString))); // Ensure you have a slot to handle the status
               QString subject = "Welcome to Our Service!";
               QString body = "Dear " + nom + ",\n\nWelcome to our service. We're glad to have you with us.\n\nBest,\nThe Team";
               smtp->sendMail("tassounaassoula@gmail.com", email, subject, body);

           }
        ui->stackedWidget->setCurrentWidget(ui->listeclient);

}
//---------------------------------------------------------------------------------------------------

void SecDialog::on_supprimersalle_clicked()
{
    int id_salle = ui->lineEdit_14->text().toInt();

    Salle salleInfo = Salle::getSalleFromDatabase(id_salle);

    bool test = salleInfo.supprimer(id_salle);

    if(test)
    {
        int capacite = salleInfo.getCapacite();
        double prix_jour = salleInfo.getPrixJour();
        QString etat = salleInfo.getEtat();
        double temp_max = salleInfo.getTemp_max();


            QFile file("C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichiersalle/supprimersalle.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture !");
            return;
        }

        QTextStream out(&file);
        QDateTime dateTime = QDateTime::currentDateTime();
        out << "ID de la salle supprimée : " << id_salle << ", Capacité : " << capacite << ", Prix par jour : " << prix_jour <<  ", Etat : " << etat << ", Température maximale : " << temp_max << ", Date de suppression : " << dateTime.toString("yyyy-MM-dd hh:mm:ss") << "\n";

        file.close();

        ui->tablesalle->setModel(salleInfo.afficher());
        QMessageBox::information(this, "Succès", "Suppression effectuée.");
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Suppression non effectuée.");
    }
}










void SecDialog::on_ajoutersalle_clicked()
{
    int capacite = ui->spinBoxsalle->value();
    double prix_jour = ui->doubleSpinBoxsalle->value();
    QString etat = "non reserve";
    double temp_max = ui->doubleSpinBoxsalle_tmp->value();
    QDateTime dateTime = QDateTime::currentDateTime();

    qDebug() << "Capacité: " << capacite;
    qDebug() << "Prix Jour: " << prix_jour;
    qDebug() << "État: " << etat;
    qDebug() << "Temp_max: " << temp_max;

    if (capacite == 0 || prix_jour == 0.0|| temp_max == 0.0) {
        QMessageBox::warning(this, "Erreur", "La capacité et le prix doivent être différents de zéro !");
        return;
    }

    Salle salle(0, capacite, prix_jour, etat,temp_max);

    if (salle.ajouter())
    {
    QFile file("C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichiersalle/salle.txt");        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture !");
            return;
        }

        QTextStream out(&file);

        QSqlQuery query;
        if (query.exec("SELECT MAX(id_salle) FROM salle")) {
            query.next();
            int idSalleAjoutee = query.value(0).toInt();
            out << "ID de la salle ajoutée : " << idSalleAjoutee << ", Capacite : " << capacite << ", Prix par jour : " << prix_jour << ", Etat : " << etat << ", temperature maximale : " << temp_max << ", Date d'ajout : " << dateTime.toString("yyyy-MM-dd hh:mm:ss") << "\n";
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'obtention de l'ID de la salle ajoutée !");
        }

        file.close();

        ui->tablesalle->setModel(salle.afficher());
        QMessageBox::information(this, "Succès", "Salle ajoutée avec succès !");
        ui->stackedWidget->setCurrentWidget(ui->listesalle);
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout de la salle !");
    }
}


void SecDialog::on_modifsalle_2_clicked()
{
    int id_salle = ui->lineEdit_14->text().toInt();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT capacite, prix_jour, etat ,temp_max FROM salle WHERE id_salle = :id_salle");
    checkQuery.bindValue(":id_salle", id_salle);

    if (checkQuery.exec() && checkQuery.next()) {
        int capacite = checkQuery.value(0).toInt();
        double prix_jour = checkQuery.value(1).toDouble();
        QString etat = checkQuery.value(2).toString();
        double temp_max = checkQuery.value(3).toDouble();

        ui->lineEditIdSalle_4->setText(QString::number(id_salle));
        ui->spinBoxsalle_4->setValue(capacite);
        ui->doubleSpinBoxsalle_4->setValue(prix_jour);
        ui->comboBoxsalle_4->setCurrentText(etat);
        ui->doubleSpinBoxsalle_tmp_2->setValue(temp_max);

        ui->lineEditIdSalle_4->setReadOnly(true);

        ui->stackedWidget->setCurrentWidget(ui->modifsalle);
    } else {
        QMessageBox::warning(this, "Erreur", "ID de salle non trouvé dans la base de données !");
    }
}


void SecDialog::on_exporter_clicked()
{
    Salle salle;

    int id_salle = ui->lineEdit_14->text().toInt();
    QString fileName = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/pdfsalle/salle_info_" + QString::number(id_salle) + ".pdf";

    QSqlQueryModel *model = salle.exporterSalleInfoToPDF(id_salle);
    if (model) {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPainter painter;
        painter.begin(&printer);
        QString idSalle = model->index(0, 0).data().toString();
        painter.drawText(100, 60, "ID de la salle: " + idSalle);

        for (int row = 0; row < model->rowCount(); ++row) {
            QString capacite = model->index(row, 0).data().toString();
            QString prix_jour = model->index(row, 1).data().toString();
            QString etat = model->index(row, 2).data().toString();
            QString temp_max = model->index(row, 3).data().toString(); // Ajout de la température maximale

            painter.drawText(100, 80 + row * 20, "Capacité: " + capacite);
            painter.drawText(100, 100 + row * 20, "Prix par jour: " + prix_jour);
            painter.drawText(100, 120 + row * 20, "État: " + etat);
            painter.drawText(100, 140 + row * 20, "Température maximale: " + temp_max); // Affichage de la température maximale
        }

        painter.end();

        qDebug() << "Informations de la salle exportées avec succès vers le fichier PDF: " << fileName;
    } else {
        qDebug() << "Erreur lors de la récupération des informations de la salle pour l'exportation.";
    }
}





void SecDialog::on_trier_clicked()
{
    Salle salle;
    QSqlQueryModel *model = salle.trierSalleParPrix();
    ui->tablesalle->setModel(model);
}

void SecDialog::on_chercher_clicked() {
    int capaciteMin = ui->lineEdit_17->text().toInt();

    Salle salle;
    QSqlQueryModel* modelSalle = salle.rechercherSallesParCapacite(capaciteMin);

    if (modelSalle) {
        ui->tablesalle->setModel(modelSalle);
    } else {
        qDebug() << "Erreur lors de la recherche de salles par capacité.";
    }
}



void SecDialog::on_mapFrame_clicked()
{
    QQuickView *qmlView = new QQuickView();
    qmlView->setSource(QUrl("qrc:/map.qml"));

    qmlView->show();
}




void SecDialog::on_modifiersalles_clicked()
{
    Salle salle;

     QList<Salle> salles = salle.getSallesFromDatabase(); // Appelez la fonction membre
    int idSalleModifiee = ui->lineEdit_14->text().toInt();

    int nouvelleCapacite = ui->spinBoxsalle_4->value();
    double nouveauPrixJour = ui->doubleSpinBoxsalle_4->value();
    QString nouvelEtat = ui->comboBoxsalle_4->currentText();
    double nouveautmp = ui->doubleSpinBoxsalle_tmp_2->value();

    if (nouvelleCapacite <= 0 || nouveauPrixJour <= 0.0 || nouveautmp <= 0.0) {
        QMessageBox::warning(this, "Erreur", "La capacité et le prix doivent être supérieurs à zéro !");
        return;
    }


    QFile file("C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichiersalle/modifiersalle.txt");

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture !");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        lines << in.readLine();
    }

    QDateTime dateTime = QDateTime::currentDateTime();

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines.at(i);
        QStringList fields = line.split(",");
        if (!fields.isEmpty() && fields.size() > 4) {
            int id = fields.at(0).split(":").last().trimmed().toInt();
            if (id == idSalleModifiee) {
                QString nombreModificationsStr = fields.last().trimmed();
                int nombreModifications = nombreModificationsStr.split(":").last().trimmed().toInt();
                if (nombreModifications > 0) {
                    lines[i] = "ID: " + QString::number(idSalleModifiee) + ", capacite: " + QString::number(nouvelleCapacite) + ", etat: " + nouvelEtat + ", prix_jour: " + QString::number(nouveauPrixJour) +", temperature maximale: " + QString::number(nouveautmp)+ ", nombre_modifications: " + QString::number(nombreModifications + 1) + ", date_derniere_modification: " + dateTime.toString("yyyy-MM-dd hh:mm:ss");
                } else {
                    lines[i] = "ID: " + QString::number(idSalleModifiee) + ", capacite: " + QString::number(nouvelleCapacite) + ", etat: " + nouvelEtat + ", prix_jour: " + QString::number(nouveauPrixJour) + ", temperature maximale: " + QString::number(nouveautmp) + " nombre_modifications: 1, date_derniere_modification: " + dateTime.toString("yyyy-MM-dd hh:mm:ss");
                }
                break;
            }
        }
    }

    file.resize(0); // Efface le contenu du fichier
    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }

    // Fermer le fichier
    file.close();


    bool modificationReussie = salle.modifier(idSalleModifiee, nouvelleCapacite, nouveauPrixJour, nouvelEtat, nouveautmp);

    if (modificationReussie) {
        ui->tablesalle->setModel(salle.afficher());

        QMessageBox::information(this, "Succès", "Salle modifiée avec succès !");
        ui->stackedWidget->setCurrentWidget(ui->listesalle);

    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la modification de la salle !");
    }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SecDialog::on_pushButton_ajoutevid_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionner une vidéo", QDir::homePath(), "Fichiers vidéo (*.mp4 *.avi *.mov)");

    if (!filePath.isEmpty()) {
        ui->lineEdit_videoPath->setText(filePath);
    }
}

void SecDialog::on_pushButton_ajoutequipement_clicked()
{
    QString description = ui->lineEdit_descriptionequi->text();
    QString designation = ui->lineEdit_designation->text();
    QString etat = ui->comboBox_etat->currentText();
    QString categorie = ui->comboBox_categorie->currentText();
    QString cout_str = ui->lineEdit_coutequi->text();
    QString videoPath = ui->lineEdit_videoPath->text();

    if (description.isEmpty() || designation.isEmpty() || cout_str.isEmpty()) {
        ui->label_message->setText("Veuillez remplir tous les champs requis.");
        return;
    }

    int cout = cout_str.toInt();

    if (cout == 0) {
        ui->label_message->setText("Veuillez saisir des données valides pour les champs numériques.");
        return;
    }

    // Créer un objet equipement en incluant le chemin du fichier vidéo
    equipement E(0, designation, cout, categorie, description, etat, videoPath);

    bool test = E.ajouterequipement();

    if (test) {
        ui->tableView_equipement->setModel(E.afficherequipement());
        ui->stackedWidget->setCurrentWidget(ui->listeequipement);
    } else {
        qDebug() << "Échec de l'ajout d'équipement.";
    }
}


void SecDialog::on_pushButton_deleteEQui_clicked()
{
    int id_equipement = ui->lineEdit_search->text().toInt();

    // Récupérer le chemin de la vidéo associée à l'équipement
    QSqlQuery query;
    query.prepare("SELECT video_equipement FROM equipement WHERE id_equipement = :id");
    query.bindValue(":id", id_equipement);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération du chemin de la vidéo.";
        return;
    }

    if (query.next()) {
        QByteArray videoData = query.value(0).toByteArray();
        // Supprimer la vidéo de la base de données
        // ... code pour supprimer la vidéo, si nécessaire
    }

    // Supprimer l'équipement de la base de données
    bool test = equi.supprimerequipement(id_equipement);

    if (test) {
        ui->tableView_equipement->setModel(equi.afficherequipement());
    } else {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Suppression non effectuée. Cliquez sur Annuler pour sortir."), QMessageBox::Cancel);
    }
}

void SecDialog::on_pushButton_modifyequi_clicked()
{
    int id_equipement = ui->lineEdit_search->text().toInt();

    if (id_equipement <= 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez saisir un identifiant valide."));
        return;
    }

    QModelIndexList selectedIndexes = ui->tableView_equipement->selectionModel()->selectedIndexes();

    if (selectedIndexes.size() == 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez sélectionner un équipement à modifier."));
        return;
    }

    QString ancienneDescription = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 4).data().toString();
    QString ancienneDesignation = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 1).data().toString();
    int ancienCout = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 2).data().toInt();
    QString ancienneCategorie = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 3).data().toString();
    QString ancienEtat = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 5).data().toString();
    QByteArray ancienneVideo = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 6).data().toByteArray(); // Nouveau : récupérer la vidéo

    ui->lineEdit_descriptionequi_2->setText(ancienneDescription);
    ui->lineEdit_designation_2->setText(ancienneDesignation);
    ui->lineEdit_coutequi_2->setText(QString::number(ancienCout));
    ui->comboBox_categorie_2->setCurrentText(ancienneCategorie);
    ui->comboBox_etat_2->setCurrentText(ancienEtat);
    // Nouveau : afficher le chemin du fichier vidéo dans un QLineEdit ou QLabel
    ui->lineEdit_videoPath_2->setText("Chemin du fichier vidéo : " + ancienneVideo); // Notez que vous devrez peut-être afficher une représentation lisible de la vidéo

    ui->stackedWidget->setCurrentWidget(ui->modifierequipement);
}

void SecDialog::on_pushButton_save_clicked()
{
    int id_equipement = ui->lineEdit_search->text().toInt();
    QString nouvelleDescription = ui->lineEdit_descriptionequi_2->text();
    QString nouvelleDesignation = ui->lineEdit_designation_2->text();
    int nouveauCout = ui->lineEdit_coutequi_2->text().toInt();
    QString nouvelleCategorie = ui->comboBox_categorie_2->currentText();
    QString nouvelEtat = ui->comboBox_etat_2->currentText();
    QByteArray nouvelleVideo = ui->lineEdit_videoPath_2->text().toUtf8(); // Nouveau : récupérer le chemin du fichier vidéo

    bool success = equi.modifierequipement(id_equipement, nouvelleDesignation, nouveauCout, nouvelleCategorie, nouvelleDescription, nouvelEtat, nouvelleVideo); // Nouveau : passer la vidéo à la fonction modifierequipement

    if (success) {
        ui->stackedWidget->setCurrentWidget(ui->listeequipement);
        ui->tableView_equipement->setModel(equi.afficherequipement());
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("La modification de l'équipement a échoué."));
    }
}



void SecDialog::on_pushButton_exportequi_clicked()
{
    int id_equipement = ui->lineEdit_search->text().toInt();
       equi.exporterEquipementPDF(id_equipement);
}






void SecDialog::on_pushButton_diagramme_equi_clicked()
{
       QChartView* chartView = equi.genererDiagrammeStatistiqueParCategorie();
       if (chartView) {
           ui->stackedWidget->addWidget(chartView);
           ui->stackedWidget->setCurrentWidget(chartView);
       } else {
           qDebug() << "Erreur lors de la génération du diagramme statistique des équipements par catégorie.";
       }
}

void SecDialog::on_pushButton_secteur_clicked()
{


      QChartView* chartView = equi.genererDiagrammeRepartitionEquipementsParCategorie();
      if (chartView) {
          ui->stackedWidget->addWidget(chartView);
          ui->stackedWidget->setCurrentWidget(chartView);
      } else {
          qDebug() << "Erreur lors de la génération du diagramme de répartition des équipements par catégorie.";
      }
}



void SecDialog::sendSMS(QString message, QString phoneNumber) {
    QString accountSid = "ACa9db5822f90cb022d2e5295028cee5d8";
    QString authToken = "d2bb1208cae3f11cf11c9a512f6e7558";
    QString twilioNumber = "+21693673656"; // Votre numéro Twilio

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString data = "To=" + phoneNumber + "&From=" + twilioNumber + "&Body=" + message;
    QByteArray postData(data.toUtf8());
    request.setHeader(QNetworkRequest::ContentLengthHeader, postData.size());
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(accountSid).arg(authToken).toUtf8()).toBase64());

    QNetworkReply *reply = manager.post(request, postData);
    connect(reply, &QNetworkReply::finished, [=]() {
        qDebug() << "SMS sent!";
        reply->deleteLater();
    });
}



void SecDialog::on_pushButton_facture_clicked() {
       int id_equipement = ui->lineEdit_search->text().toInt();

       QSqlQuery query;
       query.prepare("SELECT cout FROM equipement WHERE Id = :id");
       query.bindValue(":id", id_equipement);
       if (query.exec() && query.next()) {
           int cout = query.value(0).toInt();

           float fraisLivraison = 0.05 * cout;
           float coutTotal = cout + fraisLivraison;

           QString facture = "Coût total de l'équipement (avec frais de livraison 5%): " + QString::number(coutTotal);

           qDebug() << facture;

           QString numero_telephone = "52904114";
           QString message = "Coût total de l'équipement (avec frais de livraison 5%): " + QString::number(coutTotal);

           sendSMS(message, numero_telephone);

       } else {
           qDebug() << "Équipement introuvable pour l'ID spécifié";
       }
}





void SecDialog::on_pushButton_noter_clicked()
{
    int id_equipement = ui->lineEdit_search->text().toInt();
        int note = ui->progressBar->value();

        if (!equi.enregistrerNoteEquipement(id_equipement, note)) {
            qDebug() << "Erreur lors de l'enregistrement de la note pour l'équipement.";
        }
}






void SecDialog::on_pushButton_statnote_clicked()
{
    QFile file("C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichierequipement/note.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur : Impossible d'ouvrir le fichier pour la lecture.";
        return;
    }

    QMap<QString, QPair<int, int>> categorieSumsCounts;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(", ");

        if (parts.size() >= 3) {
            QString categorie = parts[1].split(": ")[1];
            int note = parts[2].split(": ")[1].toInt();

            categorieSumsCounts[categorie].first += note;
            categorieSumsCounts[categorie].second++;
        }
    }

    file.close();

    QBarSet *set = new QBarSet("Moyenne de notes par catégorie");
    QStringList categories;
    for (auto it = categorieSumsCounts.begin(); it != categorieSumsCounts.end(); ++it) {
        const QString& categorie = it.key();
        int sum = it.value().first;
        int count = it.value().second;
        double moyenne = count > 0 ? static_cast<double>(sum) / count : 0.0;
        *set << moyenne;
        categories << categorie;
    }

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Moyenne de notes par catégorie d'équipement");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chart->addAxis(axisY, Qt::AlignLeft);

    QValueAxis *axisX = new QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);
}

void SecDialog::on_pushButton_video_clicked()
{
    int id_equipement = ui->lineEdit_search->text().toInt();

        QString filePath = equi.obtenirCheminVideoEquipement(id_equipement);
        if (!filePath.isEmpty()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        } else {
            qDebug() << "Aucun chemin de vidéo associé à cet équipement.";
        }

}

void SecDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}




void SecDialog::on_pushButton_cherchereqy_clicked()
{
    QString nomEquipement = ui->lineEdit_search->text();
       QSqlQueryModel* model = equi.chercherEquipementParDesignation(nomEquipement);
       if (model) {
           ui->tableView_equipement->setModel(model);
       } else {
           qDebug() << "La recherche d'équipement a échoué.";

       }
}

void SecDialog::on_pushButton_trierequi_clicked()
{
    QSqlQueryModel *model = equi.trierEquipementParCout();
    ui->tableView_equipement->setModel(model);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SecDialog::on_pushButton_ajouter_formation_clicked()
{
    QString titre = ui->lineEdit_titre->text();
    QString description = ui->textEdit_description->toPlainText();
    QString date_debut = ui->dateEdit_date->text();
    QString niveau_formation = ui->comboBox_nf->currentText();
    int cout = ui->lineEdit_cout->text().toInt();
    int id_employe = ui->comboBoxIDemployefk->currentText().toInt();
    int id_salle = ui->comboBoxIDsallefk->currentText().toInt();

    // Vérification du coût
    if (cout <= 0) {
        QMessageBox::critical(nullptr, QObject::tr("Invalid Input"),
                              QObject::tr("Le coût doit être supérieur à zéro."),
                              QMessageBox::Cancel);
        return; // Annuler l'opération si l'entrée est invalide
    }

    // Création de l'objet Formation avec nbr_likes et nbr_dislikes initialisés à 0
    Formation formation(titre, description, date_debut, niveau_formation, cout, id_employe, id_salle, 0, 0);

    // Ajout de la formation
    bool test = formation.ajouter();

    if(test)
    {
        ui->tableView_4->setModel(formation.afficher());
        ui->stackedWidget->setCurrentWidget(ui->listeformation);
        /*QMessageBox::information(nullptr,QObject::tr("OK"),
               QObject::tr("Ajout effectué \n"
                           "Click Cancel to exit."), QMessageBox::Cancel);*/
    }
    /*else
    {
        QMessageBox::critical(nullptr,QObject::tr("Not ok"),
                              QObject::tr("Ajout non effectué \n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }*/
}







void SecDialog::on_pushButton_modifier_formation_clicked()
{

    int id_formation = ui->lineEdit_IDF->text().toInt();


    if (id_formation <= 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez saisir un identifiant valide."));
        return;
    }


    QModelIndexList selectedIndexes = ui->tableView_4->selectionModel()->selectedIndexes();


    if (selectedIndexes.size() == 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez sélectionner une formation à modifier."));
        return;
    }


    QString ancienTitre = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 1).data().toString();
    QString ancienneDescription = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 2).data().toString();
    QString ancienneDateDebut = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 3).data().toString();
    QString ancienNiveauFormation = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 4).data().toString();
    int ancienCout = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 5).data().toInt();


    int ancienIdEmploye = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 6).data().toInt();
    int ancienIdSalles = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 7).data().toInt();


    ui->lineEdit_titre_2->setText(ancienTitre);
    ui->textEdit_description_2->setText(ancienneDescription);
    ui->dateEdit_date_2->setDate(QDate::fromString(ancienneDateDebut));
    ui->comboBox_nf_2->setCurrentText(ancienNiveauFormation);
    ui->lineEdit_cout_2->setText(QString::number(ancienCout));
    ui->comboBoxIDemployefk_2->setCurrentText(QString::number(ancienIdEmploye));
    ui->comboBoxIDsallefk_2->setCurrentText(QString::number(ancienIdSalles));


    ui->stackedWidget->setCurrentWidget(ui->modifier);
}










void SecDialog::on_pushButton_supprimer_formation_clicked()
{
    int id_formations = ui->lineEdit_IDF->text().toInt();
    bool test = formation.supprimer(id_formations);

    if(test)
    {
        ui->tableView_4->setModel(formation.afficher());
       /*QMessageBox::information(nullptr,QObject::tr("OK"),
               QObject::tr("Supprimer effectué \n"
                           "Click Cancel to exist."), QMessageBox::Cancel);*/

    }
    /*else
        QMessageBox::critical(nullptr,QObject::tr("Not ok"),
                              QObject::tr("Suppression non effectué \n"
                                          "Click Cancel to exist."), QMessageBox::Cancel);*/

}

void SecDialog::on_save_clicked()
{
    int id_formation = ui->lineEdit_IDF->text().toInt();
    QString nouveauTitre = ui->lineEdit_titre_2->text();
    QString nouvelleDescription = ui->textEdit_description_2->toPlainText();
    QString nouvelleDateDebut = ui->dateEdit_date_2->text();
    QString nouveauNiveauFormation = ui->comboBox_nf_2->currentText();
    int nouveauCout = ui->lineEdit_cout_2->text().toInt();
    int nouveauIdEmploye = ui->comboBoxIDemployefk_2->currentText().toInt();
    int nouveauIdSalles = ui->comboBoxIDsallefk_2->currentText().toInt();

    // Récupérer les valeurs de nbr_likes et nbr_dislikes depuis les champs de saisie
    int nbr_likes = ui->lineEdit_nbr_likes_2->text().toInt();
    int nbr_dislikes = ui->lineEdit_nbr_dislikes_2->text().toInt();

    // Appeler la fonction modifier de la classe Formation en passant les valeurs récupérées
    bool success = formation.modifier(id_formation, nouveauTitre, nouvelleDescription, nouvelleDateDebut, nouveauNiveauFormation, nouveauCout, nouveauIdEmploye, nouveauIdSalles, nbr_likes, nbr_dislikes);

    if (success) {
        ui->stackedWidget->setCurrentWidget(ui->listeformation);
        ui->tableView_4->setModel(formation.afficher());
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("La modification de la formation a échoué."));
    }
}


void SecDialog::exporterPDF()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Exporter en PDF"), "", tr("Fichiers PDF (*.pdf)"));
    if (filename.isEmpty())
        return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(filename);

    QTextDocument doc;
    QTextCursor cursor(&doc);
    cursor.insertText("Liste des formations :\n\n");

    // Récupérer les données de la table
    QString formationsData;
    int rows = ui->tableView_4->model()->rowCount();
    int cols = ui->tableView_4->model()->columnCount();
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QString data = ui->tableView_4->model()->data(ui->tableView_4->model()->index(row, col)).toString();
            formationsData += data + "\t";
        }
        formationsData += "\n";
    }
    cursor.insertText(formationsData);

    doc.print(&printer);
}

void SecDialog::on_pushButton_exporterF_clicked()
{
    exporterPDF();
}





void SecDialog::on_pushButton_trierF_clicked()
{
    // Appeler la fonction pour obtenir les formations triées par le coût
    Formation formation;
    QSqlQueryModel* sortedModel = formation.getSortedFormationsByCout();

    // Vérifier si le modèle trié est valide
    if (!sortedModel)
        return;

    // Mettre à jour la vue avec le modèle trié
    ui->tableView_4->setModel(sortedModel);
}


void SecDialog::on_pushButton_chercherF_clicked()
{
    // Récupérer le titre à rechercher depuis l'interface utilisateur
    QString titre = ui->lineEdit_formationcherche->text();

    // Appeler la fonction pour rechercher les formations par titre
    Formation formation;
    QSqlQueryModel* searchModel = formation.searchFormationsByTitre(titre);

    // Vérifier si le modèle de recherche est valide
    if (!searchModel)
        return;

    // Mettre à jour la vue avec le modèle de recherche
    ui->tableView_4->setModel(searchModel);
}





void SecDialog::on_pushButton_filterNextWeek_clicked() {
    QSqlQueryModel *model = formation.filtrerFormationsNextWeek();
    ui->tableView_4->setModel(model);
}





void SecDialog::on_pushButton_diagrammeFormation_clicked()
{
    // Appeler la fonction pour obtenir les données pour le graphique
    Formation formation;
    QSqlQueryModel* model = formation.getFormationsCountByMonth();

    // Créer un diagramme à barres
    QBarSeries *series = new QBarSeries();
    for (int i = 0; i < model->rowCount(); ++i) {
        QString month = model->record(i).value("MONTH").toString();
        int count = model->record(i).value("FORMATION_COUNT").toInt();
        QBarSet *set = new QBarSet(month);
        *set << count;
        series->append(set);
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nombre de formations par mois");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Créer un axe des catégories et définir les mois
    QStringList categories;
    for (int i = 0; i < model->rowCount(); ++i) {
        categories << model->record(i).value("MONTH").toString();
    }
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Créer un axe pour les valeurs (nombre de formations)
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft); // Ajouter l'axe des valeurs à gauche du graphique

    // Associer la série au bon axe des valeurs
    series->attachAxis(axisY);

    // Créer une vue graphique pour le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher le graphique dans le stacked widget dédié à la statistique de formation
    ui->stackedWidget->addWidget(chartView);
    ui->stackedWidget->setCurrentWidget(chartView);

    // Nettoyer la mémoire
    delete model;
}


void SecDialog::on_pushButton_modeWhite_clicked()
{

    this->setStyleSheet("background-image:url(:/image/background3.png);");
    for (int i = 0; i < ui->stackedWidget->count(); ++i) {
            QWidget *widget = ui->stackedWidget->widget(i);
            widget->setStyleSheet("background-image:url(:/image/background3.png);");
        }

}


void SecDialog::on_pushButton_calendrierFormation_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->calendrierformations);

    /*// Récupérer les dates des formations et les formater en vert
    QList<QDate> datesFormations = formation.getFormationDates(); // Supposons que vous avez une fonction getFormationDates() dans la classe Formation

    // Créer un objet QTextCharFormat avec la couleur verte
    QTextCharFormat greenFormat;
    greenFormat.setBackground(Qt::green);

    foreach (const QDate& date, datesFormations) {
        ui->calendarWidget_Formation->setDateTextFormat(date, greenFormat);
    }*/
}



void SecDialog::on_calendarWidget_Formation_selectionChanged()
{
    // Récupérer la date sélectionnée
    QDate selectedDate = ui->calendarWidget_Formation->selectedDate();

    // Récupérer les détails de la formation pour cette date
    Formation formation;
    QSqlQueryModel* model = formation.getFormationDetailsByDate(selectedDate);

    // Vérifier si le modèle est valide
    if (model) {
        // Afficher les détails dans le QTextEdit
        if (model->rowCount() > 0) {
            QString details;
            for (int i = 0; i < model->columnCount(); ++i) {
                details += model->record(0).value(i).toString() + "\n";
            }
            ui->textEdit_calendar->setText(details);
        } else {
            ui->textEdit_calendar->setText("Aucune formation pour cette date.");
        }
        delete model; // Libérer la mémoire allouée pour le modèle
    } else {
        ui->textEdit_calendar->setText("Erreur lors de la récupération des détails de la formation.");
    }
}
///////////////////////////////////////////////////////
void SecDialog::on_pushButton_arduino_clicked()
{
    // Get the equipment ID from the line edit
    QString equipmentID = ui->lineEdit_search->text();

    // Send the equipment ID to Arduino and retrieve the state
    int result = A.write_equipment_id_to_arduino(equipmentID);

    // Handle the result of the write operation
    if (result == 0) {
        qDebug() << "Equipment ID sent to Arduino successfully!";
    } else {
        qDebug() << "Failed to send equipment ID to Arduino!";
    }
}
void SecDialog::on_pushButton_149_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statprojet);

        // Create an instance of the projet class
        projet myProject;

        // Call the createStat function, passing the QStackedWidget pointer
        myProject.createStat(ui->stackedWidget);



}
void SecDialog::on_pushButton_98_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statprojet);

        // Create an instance of the projet class
        projet myProject;

        // Call the createStat function, passing the QStackedWidget pointer
        myProject.createStat(ui->stackedWidget);

}
void SecDialog::on_pushButton_95_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->statprojet);
    ui->stackedWidget->setCurrentWidget(ui->statprojet);

        // Create an instance of the projet class
        projet myProject;

        // Call the createStat function, passing the QStackedWidget pointer
        myProject.createStat(ui->stackedWidget);

}
void SecDialog::on_pushButton_ajouterprojet_clicked()
{
    QString nom = ui->lineEdit_nom->text().trimmed();
    QString etat = ui->comboBox_1->currentText();
    QDate date_debut = ui->dateEdit_2->date();
    QDate date_fin = ui->dateEdit_3->date();

    QString budgetString = ui->lineEdit_budget->text().trimmed();
    QString id_employe = ui->comboBox_IDemployefk->currentText();
    int id_client = ui->comboBox_IDclientfk->currentText().toInt();

    // Validate nom and prenom are letters only
    QRegExp lettersOnly("^[A-Za-z]+$");
    if (!lettersOnly.exactMatch(nom)) {
        ui->label_message_3->setText("Le nom doit contenir uniquement des lettres.");
        return;
    }

    // Validate date_debut before date_fin
    if (date_debut >= date_fin) {
        ui->label_message_3->setText("La date de début doit être antérieure à la date de fin.");
        return;
    }

    // Validate budget (numeric) input
    bool budgetValid = false;
    int budget = budgetString.toInt(&budgetValid);
    if (!budgetValid || budget <= 0) {
        ui->label_message->setText("Veuillez saisir un budget valide.");
        return;
    }

    // Validate all attributes are not empty
    if (nom.isEmpty() || etat.isEmpty() || budgetString.isEmpty() || id_employe.isEmpty() ) {
        ui->label_message_3->setText("Veuillez remplir tous les champs.");
        return;
    }

    // Generate QR code from project name
    QString text = "Project_" + nom; // Adjust as per your naming convention
    auto image = m_generator.generateQr(text); // Generate QR code from text

    // Convert the QR code image to a QByteArray
    QByteArray qrCodeByteArray;
    QBuffer buffer(&qrCodeByteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    // Save the QR code image to a file
    QString folderPath = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/qr_code";
    QDir().mkpath(folderPath); // Create directory if it doesn't exist
    QString filePath = folderPath + "/Project_" + nom + ".png"; // Construct file path
    image.save(filePath, "PNG"); // Save the image

    // Add the project with the QR code to the database
    projet P("0", nom, etat, date_debut, date_fin, budget, id_employe, id_client, qrCodeByteArray, QList<QString>());
    bool test = P.ajouter();
    if (test) {
        ui->tableView_projet->setModel(P.afficher());
        ui->stackedWidget->setCurrentWidget(ui->listeprojet);
    } else {
        ui->label_message_3->setText("Erreur lors de l'ajout du projet.");
    }
}
void SecDialog::on_pushButton_deleteprojet_clicked()
{
    QString id_projet=ui->lineEdit_4->text();
    bool test=proj.supprimer(id_projet);
    ui->tableView_projet->setModel(proj.afficher());


}

void SecDialog::on_pushButton_modifierprojet_clicked()
{
    QString id_projet = ui->lineEdit_4->text();

    // Create an instance of the projet class
    projet myProject;

    // Variables to store project details
    QString nom, etat, id_employe;
    QDate date_debut, date_fin;
    int budget, id_client;

    // Fetch project details for modification
    if (myProject.fetchProjectDetailsForModification(id_projet, nom, etat, date_debut, date_fin, budget, id_employe, id_client)) {
        // Populate the input fields with the fetched details
        ui->lineEdit_nom3->setText(nom);
        ui->lineEdit_etat3->setText(etat);
        ui->dateEdit_4->setDate(date_debut);
        ui->dateEdit_5->setDate(date_fin);
        ui->lineEdit_budget3->setText(QString::number(budget));

        // Switch to the "modifierprojet" widget to allow modification
        ui->stackedWidget->setCurrentWidget(ui->modifierprojet);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Failed to fetch project details for modification."),
                              QMessageBox::Ok);
    }
}


void SecDialog::on_pushButton_modifierprojet2_clicked()
{
    QString id_projet = ui->lineEdit_4->text();
    QString nom = ui->lineEdit_nom3->text().trimmed();
    QString etat = ui->lineEdit_etat3->text().trimmed();
    QDate date_debut = ui->dateEdit_4->date();
    QDate date_fin = ui->dateEdit_5->date();
    QString budgetString = ui->lineEdit_budget3->text().trimmed();
    QString id_employe = ui->comboBox_IDemployefk_2->currentText();
    int id_client = ui->comboBox_IDclientfk_2->currentText().toInt();

    // Validate nom and etat are letters only
    QRegExp lettersOnly("^[A-Za-z]+$");
    if (!lettersOnly.exactMatch(nom) ) {
        ui->label_message1->setText("Le nom et l'état doivent contenir uniquement des lettres.");
        return;
    }

    // Validate date_debut before date_fin
    if (date_debut >= date_fin) {
        ui->label_message1->setText("La date de début doit être antérieure à la date de fin.");
        return;
    }

    // Validate budget (numeric) input
    bool budgetValid = false;
    int budget = budgetString.toInt(&budgetValid);
    if (!budgetValid || budget <= 0) {
        ui->label_message1->setText("Veuillez saisir un budget valide.");
        return;
    }

    // Validate all attributes are not empty
    if (nom.isEmpty() || etat.isEmpty() || budgetString.isEmpty() || id_employe.isEmpty()) {
        ui->label_message1->setText("Veuillez remplir tous les champs.");
        return;
    }

    // Call the modification function
    proj.modifier(id_projet, nom, etat, date_debut, date_fin, budget, id_employe,id_client);
    ui->tableView_projet->setModel(proj.afficher());
    ui->stackedWidget->setCurrentWidget(ui->listeprojet);
}
void SecDialog::on_pushButton_trierprojet_clicked()
{
    // Create an instance of the projet class
    projet myProject;

    // Sort the projects by budget
    QSqlQueryModel* model = myProject.sortProjectsByBudget();

    // Update the view model
    ui->tableView_projet->setModel(model);
}
void SecDialog::on_pushButton_exportprojet_clicked()
{
    // Récupérer l'ID de l'équipement à exporter
    int id_projet = ui->lineEdit_4->text().toInt();

    // Create an instance of the projet class
    projet myProject;

    // Export project information to a PDF file
    myProject.exportProjectInfoToPDF(id_projet);
}





void SecDialog::on_pushButton_162_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->reservation);

}

void SecDialog::on_pushButton_ajouter_3_clicked()
{
    QString id_equipement = ui->comboBox_IDequipements->currentText();
    QString id_projet = ui->comboBox_IDprojets->currentText();
    QString heure = ui->timeEdit_h->time().toString("HH:mm"); // Ensure time format is correct
    QDate dates = ui->dateEdit_d1->date();

    // Check for empty fields
    if (id_equipement.isEmpty() || id_projet.isEmpty() || heure.isEmpty()) {
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }

    // Create a reservation object
    reservation R(id_equipement, id_projet, dates, heure);

    // Attempt to add the reservation to the database
    bool test = R.ajouter();
    if (test) {
        // Update the state of the equipment to "reserved"
        QString error;
        if (!R.updateEquipmentState("reserve", error)) {
            QMessageBox::critical(this, "Error", "Failed to update equipment state: " + error);
            return;
        }

        ui->stackedWidget->setCurrentWidget(ui->listereservation);
    } else {
        QMessageBox::critical(this, "Error", "Failed to add reservation to the database.");
    }
    ui->tableView_reservation->setModel(reserve.afficherreservation());

}


void SecDialog::on_pushButton_chercherprojet_clicked()
{
    // Retrieve the name of the project to search for
    QString nomProjet = ui->lineEdit_4->text();

    // Create an instance of the projet class
    projet myProject;

    // Perform the search by project name
    QSqlQueryModel* model = myProject.searchProjectByName(nomProjet);

    // Update the view model
    ui->tableView_projet->setModel(model);
}
void SecDialog::on_pushButton_trier_2_clicked()
{
    // Create an instance of the projet class
    projet myProject;

    // Sort the projects by budget
    QSqlQueryModel* model = myProject.sortProjectsByBudget();

    // Update the view model
    ui->tableView_projet->setModel(model);
}
void SecDialog::on_pushButton_163_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->scanprojet);

}
void SecDialog::on_pushButton_scan_clicked()
{
    static bool codeExecuted = false; // Static variable to track if the code has been executed

    // If the code has already been executed, return without doing anything
    if (codeExecuted) {
        return;
    }

    // Execute the Python script using QProcess
    QProcess *process = new QProcess(this);

    // Set the path to your Python interpreter (Thonny)
    QString pythonInterpreterPath = "D:/Thonny/python.exe";

    // Set the path to your Python script
    QString pythonScriptPath = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/nou.py"; // Adjust this path to the location of your Python script

    // Start the Python script using the specified Python interpreter
    process->start(pythonInterpreterPath, QStringList() << pythonScriptPath);

    // Wait for the process to finish (optional)
    process->waitForFinished();

    // Read the output and error streams (optional)
    QString output = process->readAllStandardOutput().trimmed(); // Trimmed to remove leading/trailing spaces and newline

    // Clean up the process
    delete process;

    // Set the path to the folder containing the QR code PNG files
    QString folderPath = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/qr_code";

    // Get a list of all files in the folder
    QDir folderDir(folderPath);
    QStringList fileNames = folderDir.entryList(QStringList() << "*.png", QDir::Files);

    // Prepare the list of QR code names
    QStringList qrCodeNames;
    for (const QString &fileName : fileNames) {
        qrCodeNames.append(fileName.mid(8, fileName.lastIndexOf(".") - 8)); // Extract the project name from the file name
    }

    // Construct the label text
    QString labelText = "Python output: " + output + "\n";
    labelText += "List of QR code names: " + qrCodeNames.join(", ") + "\n";

    // Check if the output matches any of the expected QR code names (file names)
    QString projectName; // Variable to store the name of the project
    for (const QString &fileName : fileNames) {
        if (fileName.startsWith("Project_" + output)) {
            projectName = fileName.mid(8, fileName.lastIndexOf(".") - 8); // Extract the project name from the file name
            break; // Exit the loop if a match is found
        }
    }

    // If a matching project name was found
    if (!projectName.isEmpty()) {
        qDebug() << "Access granted for project:" << projectName;

        // Create an instance of the projet class
        projet project;

        // Retrieve the corresponding project ID from the database based on the project name
        QString projectId = project.getProjectIdFromName(projectName); // Call the method to retrieve project ID from the project name

        // Append the project ID to the label text
        labelText += "Project ID: " + projectId + "\n";

        // Load tasks for the project using the instance
        QList<QString> tasks = project.getTasksForProject(projectId);

        // Display tasks in the appropriate QTextEdit widgets
        for (int i = 0; i < tasks.size(); ++i) {
            if (i == 0) {
                ui->textEdit->setPlainText(tasks[i]);
            } else if (i == 1) {
                ui->textEdit_2->setPlainText(tasks[i]);
            } else if (i == 2) {
                ui->textEdit_3->setPlainText(tasks[i]);
            }
        }
    } else {
        qDebug() << "Output does not match any of the expected QR code names.";
        labelText += "No matching project found for the scanned QR code.\n";
    }

    // Set the text of the label_scan to the constructed label text
    ui->label_scan->setText(labelText);

    // Set the flag to true to indicate that the code has been executed
    codeExecuted = true;
}



void SecDialog::on_pushButton_168_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->scanprojet);

}





void SecDialog::on_pushButton_169_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->reservation);

}

void SecDialog::on_pushButton_up_clicked()
{
    int id = ui->lineEdit_10->text().toInt();

        // Récupérer les anciennes valeurs depuis l'interface graphique

        QString anciennom = ui->lineEdit_nom->text();
        QString ancienprenom = ui->lineEdit_prenom->text();
        QString ancienemail = ui->lineEdit_email->text();
        QString ancienadresse = ui->lineEdit_adresse->text();
        QString anciensexe = ui->lineEdit_sexe->text();

        QString anciendateNaissance = ui->dateEdit_DDN->text();
        QString anciendateInscription = ui->dateEdit_inscrit->text();

        // Récupérer les nouvelles valeurs depuis l'interface graphique
         QString nouvnom = ui->lineEdit_nom_up->text();
         QString nouvprenom = ui->lineEdit_prenom_up->text();
         QString nouvemail = ui->lineEdit_email_up->text();
         QString nouvadresse = ui->lineEdit_adresse_up->text();
         QString nouvsexe = ui->lineEdit_sexe_up->text();
    QString nouvdateInscription= ui->dateEdit_inscrit_up->text();
    QString nouvdateDeNaissanceDate= ui->dateEdit_DDN_up->text();



        // Vérifier s'il y a eu une modification
        if (anciennom != nouvnom || ancienprenom != nouvprenom || ancienemail != nouvemail|| ancienadresse != nouvadresse|| anciensexe != nouvsexe|| anciendateNaissance != nouvdateDeNaissanceDate|| anciendateInscription != nouvdateInscription) {
            // Il y a eu une modification, mettre à jour les détails de la salle dans la base de données
            QByteArray nouvelleImage;
            QBuffer buffer(&nouvelleImage);
            buffer.open(QIODevice::WriteOnly);
           // ui->label_image->pixmap()->save(&buffer, "PNG");
            Client client(id, nouvsexe, nouvnom, nouvprenom, nouvadresse, nouvemail, nouvdateDeNaissanceDate, nouvdateInscription, nouvelleImage);
            bool modificationReussie = client.modifier(id, nouvnom, nouvprenom,nouvsexe,nouvemail, nouvadresse,nouvdateDeNaissanceDate,nouvdateInscription);

            if (modificationReussie) {
                // Rafraîchir les données affichées (si nécessaire)
                ui->tableView->setModel(client.afficher());

                // Optionnellement, afficher un message de succès ou effectuer d'autres actions
                //QMessageBox::information(nullptr, "Succès", "client modifiée avec succès !");

                ui->stackedWidget->setCurrentWidget(ui->listeclient);
            } else {
                // Gérer le cas où la modification de la salle a échoué
                //QMessageBox::warning(nullptr, "Erreur", "Erreur lors de la modification de client !");
            }
        } else {
            // Aucune modification n'a été apportée
            //QMessageBox::critical(nullptr, "Information", "Aucune modification n'a été apportée.");
        }
}

float SecDialog::getTempMaxFromDB(int salleId)
{
    QSqlQuery query;
    query.prepare("SELECT temp_max FROM salle WHERE id_salle = :id");
    query.bindValue(":id", salleId);

    if (!query.exec()) {
        qDebug() << "Failed to query database:" << query.lastError();
        return -1; // return an error value
    }

    if (query.next()) {
        return query.value(0).toFloat();
    } else {
        qDebug() << "No record found for salle ID:" << salleId;
        return -1; // return an error value
    }
}
void SecDialog::on_ventilateur_clicked()
{
    int salleId = ui->lineEdit_14->text().toInt();
        float maxTemp = getTempMaxFromDB(salleId);
        float currentTemperature = A.readTemperature();

        qDebug() << "Max Temp from DB:" << maxTemp << "Current Temp:" << currentTemperature;

        if (maxTemp == -1) {
            ui->labelResult->setText("ID de la salle non trouvé ou erreur de base de données.");
        } else if (std::isnan(currentTemperature)) {
            ui->labelResult->setText("Erreur de lecture de la température.");
        } else if (currentTemperature > maxTemp) {
            ui->labelResult->setText("La température est supérieure à la température maximale. Il faut démarrer le ventilateur.");
            A.write_to_arduino("1");  // Envoie "1" à l'Arduino pour activer le ventilateur
        } else {
            ui->labelResult->setText("La température est dans les limites normales.");
            A.write_to_arduino("0");  // Envoie "0" à l'Arduino pour désactiver le ventilateur
        } }

void SecDialog::on_pushButton3_clicked()
{
    if (!A.store_serial_data()) {
            qDebug() << "Failed to store serial data!";
            return;
        }

        //ui->lineEditarduino->setText(A.get_data());

        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isValid()) {
            qDebug() << "Database connection invalid!";
            return;
        }

        QSqlQuery query;
        query.prepare("SELECT designation FROM equipement WHERE id_equipement = :id");
        query.bindValue(":id", A.get_data().toInt());
        if (query.exec() && query.next()) {
            // Récupérer la désignation de l'équipement
            QString designation = query.value(0).toString();

            ui->lineEditarduino_2->setText(designation);
        } else {
            qDebug() << "Failed to fetch equipment designation from database!";
        }
}

void SecDialog::onSerialDataReceived() {
    if (A.store_serial_data()) {
        QString dataString = A.get_data();
        if (dataString.contains("E")) {
            dataString.replace("E", "");

            QSqlDatabase db = QSqlDatabase::database();
            if (!db.isValid()) {
                qDebug() << "Database connection invalid!";
                return;
            }

            QSqlQuery query;
            query.prepare("SELECT designation FROM equipement WHERE id_equipement = :id");
            query.bindValue(":id", dataString.toInt());
            if (query.exec() && query.next()) {
                QString designation = query.value(0).toString();
                ui->lineEditarduino_2->setText(designation);

                // Send the equipment name to Arduino
                A.write(designation);
            } else {
                qDebug() << "Failed to fetch equipment designation from database!";
            }
        }
    }
}

void SecDialog::on_pushButton_arduino1_clicked()
{
    //Get the equipment ID from the line edit
        QString equipmentID = ui->lineEdit_arduino->text();

        // Send the equipment ID to Arduino and retrieve the state
        int result = A.write_equipment_id_to_arduino(equipmentID);

        // Handle the result of the write operation
        if (result == 0) {
            qDebug() << "Equipment ID sent to Arduino successfully!";
        } else {
            qDebug() << "Failed to send equipment ID to Arduino!";
        }
}
