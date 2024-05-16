#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "secdialog.h"
#include "ui_secdialog.h"
#include<projet.h>
#include<formation.h>
#include <employe.h>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include "arduino.h"
#include <QPrinter>
#include <QPainter>
#include <QThread>

#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_mdp_login->setEchoMode(QLineEdit::Password);
    ui->lineEdit_nouveau_mdp->setEchoMode(QLineEdit::Password);

    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
     //le slot update_label suite à la reception du signal readyRead (reception des données).




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_label()
{
    data = A.read_from_arduino();
    qDebug() << "Data reçue de l'Arduino :" << data;

    // Supprimer les caractères spéciaux de la chaîne de données
        data.replace("\r", "");  // Supprimer les retours chariots
        // Vérifier si la chaîne de données contient uniquement des chiffres
            if (!data.isEmpty() && data.toInt() != 0) {
                int id = data.toInt(); // Convertir la donnée reçue de l'Arduino en entier pour l'ID
                qDebug() << "ID converti en entier :" << id;

                employe employe;
                if (employe.verifierExistenceEmployeParID(id)) {
                    ui->label_arduino->setText("Existe"); // Si l'employé avec cet ID existe, afficher "Existe", l'ID et la data
                    A.write_to_arduino("0");  // Envoyer 0 à Arduino

                    // Définir un délai de 5 secondes avant d'envoyer le signal "1" à Arduino
                    QTimer::singleShot(5000, [this]() {
                        A.write_to_arduino("1");
                    });
                } else {
                    ui->label_arduino->setText("N'existe pas"); // Si l'employé avec cet ID n'existe pas, afficher "N'existe pas", l'ID et la data
                }
            }
        }






void MainWindow::on_pushButton_5_clicked()
{
    if (posteEmployeConnecte == "Responsable employe" || posteEmployeConnecte == "admin") {
        hide();
        secDialog = new SecDialog(this);
        secDialog->setPosteEmployeConnecte(posteEmployeConnecte);
        secDialog->show();
        secDialog->setCurrentPage(30); // Assuming page_5 is at index 4
    } else {
        ui->label_message_menu->setText("Vous n'avez pas les autorisations nécessaires pour accéder à cette fonctionnalité.");
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    if (posteEmployeConnecte == "Responsable projets" || posteEmployeConnecte == "admin") {
        hide();
        secDialog = new SecDialog(this);
        secDialog->setPosteEmployeConnecte(posteEmployeConnecte);
        secDialog->show();
        secDialog->setCurrentPage(26); // Assuming page_5 is at index 4
    } else {
        ui->label_message_menu->setText("Vous n'avez pas les autorisations nécessaires pour accéder à cette fonctionnalité.");
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    if (posteEmployeConnecte == "Responsable équipements" || posteEmployeConnecte == "admin") {
        hide();
        secDialog = new SecDialog(this);
        secDialog->setPosteEmployeConnecte(posteEmployeConnecte);
        secDialog->show();
        secDialog->setCurrentPage(31); // Assuming page_5 is at index 4
    } else {
        ui->label_message_menu->setText("Vous n'avez pas les autorisations nécessaires pour accéder à cette fonctionnalité.");
    }
}
void MainWindow::on_pushButton_10_clicked()
{
    if (posteEmployeConnecte == "Responsable client" || posteEmployeConnecte == "admin") {
        hide();
        secDialog = new SecDialog(this);
        secDialog->setPosteEmployeConnecte(posteEmployeConnecte);
        secDialog->show();
        secDialog->setCurrentPage(25); // Assuming page_5 is at index 4
    } else {
        ui->label_message_menu->setText("Vous n'avez pas les autorisations nécessaires pour accéder à cette fonctionnalité.");
    }
}
void MainWindow::on_pushButton_11_clicked()
{
    if (posteEmployeConnecte == "Responsable salles" || posteEmployeConnecte == "admin") {
        hide();
        secDialog = new SecDialog(this);
        secDialog->setPosteEmployeConnecte(posteEmployeConnecte);
        secDialog->show();
        secDialog->setCurrentPage(33); // Assuming page_5 is at index 4
    } else {
        ui->label_message_menu->setText("Vous n'avez pas les autorisations nécessaires pour accéder à cette fonctionnalité.");
    }

}
void MainWindow::on_pushButton_12_clicked()
{
    if (posteEmployeConnecte == "Responsable formation" || posteEmployeConnecte == "admin") {
        hide();
        secDialog = new SecDialog(this);
        secDialog->setPosteEmployeConnecte(posteEmployeConnecte);
        secDialog->show();
        secDialog->setCurrentPage(32); // Assuming page_5 is at index 4
    } else {
        ui->label_message_menu->setText("Vous n'avez pas les autorisations nécessaires pour accéder à cette fonctionnalité.");
    }
}

void MainWindow::on_pushButton_connection_clicked()
{
    employe employe;
    QString email = ui->lineEdit_mail_login->text();
    QString password = ui->lineEdit_mdp_login->text();
    QString poste;

    if (employe.verifierConnexion(email, password, poste)) {

        posteEmployeConnecte = poste;

        ui->stackedWidget->setCurrentIndex(2);
    } else {

        ui->label_message_login->setText("mail ou mot de passe incorrects.");
    }
}

void MainWindow::on_pushButton_modifier_mdp_clicked()
{
    employe employe;
    QString email = ui->lineEdit_email_mdp_oublie->text();
    QString questionUtilisateur = ui->comboBox_question_new->currentText();
    QString reponseUtilisateur = ui->lineEdit_reponse->text();
    QString nouveauMdp = ui->lineEdit_nouveau_mdp->text();

    if (employe.modifierMotDePasse(email, questionUtilisateur, reponseUtilisateur, nouveauMdp)) {

        ui->label_message_modifier_mdp->setText("Mot de passe mis à jour avec succès.");
    } else {

        if (employe.modifierMotDePasse(email, questionUtilisateur, reponseUtilisateur, nouveauMdp)) {
            ui->label_message_modifier_mdp->setText("Erreur lors de la mise à jour du mot de passe.");
        } else {
            ui->label_message_modifier_mdp->setText("Question ou réponse incorrecte.");
        }
    }
}

void MainWindow::on_pushButton_mdpoublier_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_loginPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_deconection_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_ouvrir_clicked()
{
    /*// Récupérer l'ID de l'employé depuis lineEdit_ouvrir
    int id_employe = ui->lineEdit->text().toInt();

    // Vérifier si l'employé avec cet ID existe
    employe emp;
    bool existe = emp.verifierExistenceEmployeParID(id_employe);

    // Si l'employé existe, envoyer 0 à Arduino et attendre 5 secondes
    if (existe) {
        A.write_to_arduino("0");  // Envoyer 0 à Arduino

        // Définir un délai de 5 secondes avant d'envoyer le signal "1" à Arduino
        QTimer::singleShot(5000, [this]() {
            A.write_to_arduino("1");
        });
    } else {
        qDebug() << "Aucun employé avec l'ID" << id_employe << "n'a été trouvé.";
        // Ajoutez ici la logique pour traiter le cas où l'employé n'existe pas
    }*/
}
