#include "equipement.h"
#include <QSqlQuery>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QDir>
#include <QtCharts>
#include <QLineSeries>
#include <QtMath>
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


equipement::equipement(int id_equipement, QString designation, int cout, QString categorie, QString description, QString etat,  QString video_equipement)
{
    this->id_equipement = id_equipement;
    this->designation = designation;
    this->cout = cout;
    this->categorie = categorie;
    this->description = description;
    this->etat = etat;
    this->video_equipement = video_equipement; // Affectation de la vidéo
}


bool equipement::ajouterequipement()
{
    QSqlQuery query;
    QString res = QString::number(id_equipement);
    query.prepare("INSERT INTO equipement (id_equipement, designation, cout, categorie, description, etat, video_equipement) "
                  "VALUES (:id_equipement, :designation, :cout, :categorie, :description, :etat, :video_equipement)");
    query.bindValue(":id_equipement", id_equipement);
    query.bindValue(":designation", designation);
    query.bindValue(":cout", cout);
    query.bindValue(":categorie", categorie);
    query.bindValue(":description", description);
    query.bindValue(":etat", etat);
    query.bindValue(":video_equipement", video_equipement); // Nouveau bind pour la vidéo

    return query.exec();
}


QSqlQueryModel * equipement::afficherequipement()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_equipement, designation, cout, categorie, description, etat, video_equipement FROM equipement");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Designation"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Cout"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Categorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Etat"));
    // Nouvelle colonne pour la vidéo
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Video Equipement"));

    return model;
}


bool equipement::supprimerequipement(int id_equipement)
{
    QSqlQuery query;
    QString res = QString::number(id_equipement);

    query.prepare("DELETE FROM equipement WHERE id_equipement = :id_equipement");
    query.bindValue(":id_equipement", id_equipement);

    return query.exec();
}


bool equipement::modifierequipement(int id_equipement, QString designation, int cout, QString categorie, QString description, QString etat, QString video_equipement)
{
    QSqlQuery query;
    query.prepare("UPDATE equipement SET designation = :designation, cout = :cout, categorie = :categorie, description = :description, etat = :etat, video_equipement = :video_equipement WHERE id_equipement = :id_equipement");
    query.bindValue(":id_equipement", id_equipement);
    query.bindValue(":designation", designation);
    query.bindValue(":cout", cout);
    query.bindValue(":categorie", categorie);
    query.bindValue(":description", description);
    query.bindValue(":etat", etat);
    query.bindValue(":video_equipement", video_equipement); // Nouveau bind pour la vidéo

    return query.exec();
}


bool equipement::exporterEquipementPDF(int id_equipement)
{
    QSqlQuery query;
    query.prepare("SELECT designation, etat, cout, categorie, description FROM equipement WHERE id_equipement = :id_equipement");
    query.bindValue(":id_equipement", id_equipement);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des informations de l'équipement pour l'exportation.";
        return false;
    }

    if (!query.next()) {
        qDebug() << "Aucun équipement trouvé avec l'ID spécifié.";
        return false;
    }

    QString designation = query.value(0).toString();
    QString etat = query.value(1).toString();
    int cout = query.value(2).toInt();
    QString categorie = query.value(3).toString();
    QString description = query.value(4).toString();

    QString fileName = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichierequipement/equipement_info_" + QString::number(id_equipement) + ".pdf";

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    painter.begin(&printer);

    painter.drawText(100, 100, "Designation: " + designation);
    painter.drawText(100, 120, "Etat: " + etat);
    painter.drawText(100, 140, "Cout: " + QString::number(cout));
    painter.drawText(100, 160, "Catégorie: " + categorie);
    painter.drawText(100, 180, "Description: " + description);

    painter.end();

    qDebug() << "Informations de l'équipement exportées avec succès vers le fichier PDF: " << fileName;

    return true;
}
QSqlQueryModel* equipement::trierEquipementParCout()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id_equipement, designation, cout, categorie, description, etat, video_equipement FROM equipement ORDER BY cout DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Designation"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Cout"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Categorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Video Equipement")); // Nouvelle colonne pour la vidéo
    return model;
}


QSqlQueryModel* equipement::chercherEquipementParDesignation(const QString &nomEquipement)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id_equipement, designation, cout, categorie, description, etat, video_equipement FROM equipement WHERE designation LIKE :nomEquipement");
    query.bindValue(":nomEquipement", "%" + nomEquipement + "%"); // Utilisation de LIKE pour trouver les correspondances partielles
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête de recherche.";
        return nullptr;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Designation"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Cout"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Categorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Video Equipement")); // Nouvelle colonne pour la vidéo

    return model;
}

QChartView* equipement::genererDiagrammeStatistiqueParCategorie()
{
    QSqlQuery query;
    query.exec("SELECT categorie, COUNT(*) FROM equipement GROUP BY categorie");

    QBarSet *set = new QBarSet("Nombre d'équipements par catégorie");
    QStringList categories;
    QMap<QString, int> categoryCountMap;
    while (query.next()) {
        QString categorie = query.value(0).toString();
        int count = query.value(1).toInt();
        *set << count;
        categories << categorie;
        categoryCountMap.insert(categorie, count);
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques d'équipements par catégorie");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTickCount(categoryCountMap.size());
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Nombre d'équipements");
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
QChartView* equipement::genererDiagrammeRepartitionEquipementsParCategorie()
{
    QSqlQuery query;
    query.exec("SELECT categorie, COUNT(*) FROM equipement GROUP BY categorie");

    int totalEquipments = 0;

    QPieSeries *pieSeries = new QPieSeries();
    while (query.next()) {
        QString categorie = query.value(0).toString();
        int count = query.value(1).toInt();
        totalEquipments += count;
        QPieSlice *slice = new QPieSlice(categorie, count);
        pieSeries->append(slice);
    }

    double totalPercentage = 0.0;
    for (QPieSlice *slice : pieSeries->slices()) {
        double slicePercentage = (slice->percentage() / 100.0) * totalEquipments;
        totalPercentage += slicePercentage;
    }

    for (QPieSlice *slice : pieSeries->slices()) {
        double slicePercentage = (slice->percentage() / 100.0) * totalEquipments;
        slice->setLabel(QString("%1 (%2%)").arg(slice->label()).arg(slicePercentage / totalPercentage * 100));
    }

    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Répartition des équipements par catégorie (Secteurs empilés)");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
bool equipement::enregistrerNoteEquipement(int id_equipement, int note)
{
    QString categorieEquipement;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Erreur : Impossible d'ouvrir la base de données.";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT categorie FROM equipement WHERE id_equipement = :id");
    query.bindValue(":id", id_equipement);
    if (query.exec() && query.next()) {
        categorieEquipement = query.value(0).toString();
    } else {
        qDebug() << "Erreur lors de la récupération du nom de l'équipement.";
        return false;
    }

    QFile file("C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/fichierequipement/note.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        // Créer une sortie texte pour écrire dans le fichier
        QTextStream out(&file);

        // Écrire l'ID, le nom de l'équipement et sa note dans le fichier
        out << "ID: " << id_equipement << ", categorie: " << categorieEquipement << ", Note: " << note << endl;

        // Fermer le fichier
        file.close();
        return true;
    } else {
        qDebug() << "Erreur : Impossible d'ouvrir le fichier pour l'écriture.";
        return false;
    }
}

QString equipement::obtenirCheminVideoEquipement(int id_equipement)
{
    QSqlQuery query;
    query.prepare("SELECT video_equipement FROM equipement WHERE id_equipement = :id");
    query.bindValue(":id", id_equipement);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération du chemin de la vidéo.";
        return QString();
    }

    if (query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Aucun équipement trouvé avec cet ID.";
        return QString();
    }
}




