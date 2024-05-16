  #ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include<projet.h>
#include<formation.h>
#include<employe.h>
#include<equipement.h>
#include<client.h>
#include<salle.h>
#include<arduino.h>
#include<reservation.h>
#include <QrCodeGenerator.h>
#include <QtCharts>
#include <QLineSeries>
#include <QScatterSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QPieSeries>
#include <QPieSlice>

namespace Ui {
class SecDialog;
}

class SecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecDialog(QWidget *parent = nullptr);
    void setCurrentPage(int index); // Declaration of the function
    void setPosteEmployeConnecte(const QString &poste);

    ~SecDialog();

private slots:
    void on_pushButton_237_clicked();

    void on_pushButton_238_clicked();


    void on_pushButton_239_clicked();

    void on_pushButton_144_clicked();


    void on_pushButton_145_clicked();

    void on_pushButton_146_clicked();

    void on_pushButton_84_clicked();

    void on_pushButton_85_clicked();

    void on_pushButton_86_clicked();

    void on_pushButton_87_clicked();

    void on_pushButton_93_clicked();

    void on_pushButton_88_clicked();

    void on_pushButton_147_clicked();

    void on_pushButton_150_clicked();

    void on_pushButton_153_clicked();

    void on_pushButton_156_clicked();

    void on_pushButton_159_clicked();
    void on_pushButton_148_clicked();

    void on_pushButton_149_clicked();

    void on_pushButton_151_clicked();

    void on_pushButton_152_clicked();

    void on_pushButton_154_clicked();

    void on_pushButton_155_clicked();

    void on_pushButton_157_clicked();

    void on_pushButton_158_clicked();

    void on_pushButton_160_clicked();
    void on_pushButton_161_clicked();


    void on_pushButton_97_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_95_clicked();

    void on_pushButton_90_clicked();

    void on_pushButton_89_clicked();

    void on_pushButton_142_clicked();

    void on_pushButton_143_clicked();

    void on_pushButton_138_clicked();

    void on_pushButton_140_clicked();

    void on_pushButton_136_clicked();

    void on_pushButton_135_clicked();

    void on_pushButton_134_clicked();

    void on_pushButton_133_clicked();

    void on_pushButton_129_clicked();

    void on_pushButton_131_clicked();

    void on_pushButton_127_clicked();

    void on_pushButton_126_clicked();

    void on_pushButton_124_clicked();

    void on_pushButton_125_clicked();

    void on_pushButton_120_clicked();

    void on_pushButton_122_clicked();

    void on_pushButton_117_clicked();

    void on_pushButton_118_clicked();

    void on_pushButton_115_clicked();

    void on_pushButton_116_clicked();

    void on_pushButton_111_clicked();

    void on_pushButton_113_clicked();

    void on_pushButton_109_clicked();

    void on_pushButton_108_clicked();

    void on_pushButton_106_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_102_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_99_clicked();


    void on_pushButton_ajouter_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_modifier_clicked();


    void on_pushButton_supprimeremploye_clicked();

    void on_pushButton_modifieremploye_clicked();

    void on_pushButton_ajouteremploye_clicked();

    void on_pushButton_ajouteremploye_2_clicked();

    void on_pushButton_modifierprojet2_clicked();


    void on_pushButton_editemploye_clicked();

    void on_pushButton_clicked();

    void on_pushButton_annulerajou_clicked();

    void on_pushButton_trier_clicked();

    void on_pushButton_chercheremploye_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_diagramme_clicked();

    void on_pushButton_diagramme_2_clicked();

    void on_pushButton_annulermodif_clicked();

    void on_pushButton_logout_clicked();

    void on_pushButton_modifier_2_clicked();

    void on_pushButton_chercher_client_clicked();

    void on_pushButton_exportClient_clicked();

    void on_pushButton_supprimer_clicked();

    void on_pushButton_trier_client_clicked();

    void on_pushButton_ajouter_2_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_modifier_client_clicked();

    void on_pushButton_selectImage_clicked();

    void on_ajoutersalle_clicked();

    void on_supprimersalle_clicked();


    void on_modifsalle_2_clicked();

    void on_pushButton_ajouter_3_clicked();


    void on_exporter_clicked();

    void on_trier_clicked();

    void on_chercher_clicked();

    void on_mapFrame_clicked();

    void on_modifiersalles_clicked();



    void on_pushButton_ajoutequipement_clicked();

        void on_pushButton_deleteEQui_clicked();

        void on_pushButton_modifyequi_clicked();

        void on_pushButton_save_clicked();

        void on_pushButton_exportequi_clicked();


        void on_pushButton_cherchereqy_clicked();

        void on_pushButton_diagramme_equi_clicked();


        void on_pushButton_secteur_clicked();


        void on_pushButton_facture_clicked();
        void sendSMS(QString message, QString phoneNumber);


        void on_pushButton_noter_clicked();


        void on_pushButton_statnote_clicked();


        void on_pushButton_video_clicked();

        void on_pushButton_ajoutevid_clicked();

        void on_horizontalSlider_valueChanged(int value);
        void on_pushButton_trierequi_clicked();

        void on_pushButton_filterNextWeek_clicked();

        void on_pushButton_calendrierFormation_clicked();

        void on_pushButton_trierF_clicked();

        void on_pushButton_modifier_formation_clicked();
        void exporterPDF();
        void on_pushButton_exporterF_clicked();

        void on_pushButton_supprimer_formation_clicked();

        void on_pushButton_chercherF_clicked();

        void on_pushButton_ajouter_formation_clicked();

        void on_calendarWidget_Formation_selectionChanged();

        void on_save_clicked();

        void on_pushButton_diagrammeFormation_clicked();

        void on_pushButton_modeWhite_clicked();
        void on_pushButton_ajouterprojet_clicked();

        void on_pushButton_deleteprojet_clicked();

       void on_pushButton_modifierprojet_clicked();
        void on_pushButton_exportprojet_clicked();
        void on_pushButton_arduino_clicked();
        void on_pushButton_trier_2_clicked();
        void on_pushButton_scan_clicked();
        void on_pushButton_trierprojet_clicked();
        void on_pushButton_chercherprojet_clicked();

        void on_pushButton_163_clicked();
        void on_pushButton_162_clicked();
        void on_pushButton_168_clicked();

        void on_pushButton_chercher_clicked();

        void on_pushButton_169_clicked();

        void on_pushButton_141_clicked();
        float getTempMaxFromDB(int salleId);

        void on_ventilateur_clicked();

        void on_pushButton3_clicked();
        void onSerialDataReceived();

        void on_pushButton_arduino1_clicked();

private:
    Ui::SecDialog *ui;
    Salle salles;
    QMap<int, int> modifications;

    QrCodeGenerator m_generator;

    projet proj;
    Formation formation;
    employe emp;
    equipement equi;
QString posteEmployeConnecte;
Client client;
QByteArray m_imageData;  // Private member to store image data
QByteArray mCurrentImageData;  // Private member to store image data

int temperatureManuelle;
reservation reserve;
QByteArray data; // variable contenant les données reçues

Arduino A;
};

#endif // SECDIALOG_H
