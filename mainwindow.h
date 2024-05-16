#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secdialog.h"
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
Arduino A; // objet temporaire
private slots:
void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
// ce slot est lancé à chaque réception d'un message de Arduino
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();

    void on_pushButton_connection_clicked();

    void on_pushButton_verifiermail_clicked();

    void on_pushButton_modifier_mdp_clicked();

    void on_pushButton_mdpoublier_clicked();

    void on_pushButton_loginPage_clicked();

    void on_pushButton_deconection_clicked();


    void on_pushButton_ouvrir_clicked();

private:
    Ui::MainWindow *ui;
    SecDialog *secDialog;
    QString posteEmployeConnecte;
    QByteArray data; // variable contenant les données reçues


};
#endif // MAINWINDOW_H
