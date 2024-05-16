#include "projet.h"
#include <QDebug>

projet::projet(QString id_projet, QString nom, QString etat, QDate date_debut, QDate date_fin, int budget, QString id_employe, int id_client, QByteArray qrCode, QList<QString> taches)
    : id_projet(id_projet), nom(nom), etat(etat), date_debut(date_debut), date_fin(date_fin), budget(budget), id_employe(id_employe), id_client(id_client), qrCode(qrCode), taches(taches) {}

bool projet::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO projet (nom, etat, date_debut, date_fin, budget, id_employe, id_client, qr_code, taches) "
                  "VALUES (:nom, :etat, :date_debut, :date_fin, :budget, :id_employe, :id_client, :qr_code, :taches)");
    query.bindValue(":nom", nom);
    query.bindValue(":etat", etat);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":budget", budget);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_client", id_client);
    query.bindValue(":qr_code", qrCode);
    query.bindValue(":taches", taches.join(", ")); // Assuming taches is a list of strings
    return query.exec();
}

QSqlQueryModel * projet::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_projet, nom, etat, date_debut, date_fin, budget, id_employe, id_client FROM projet");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date fin"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Budget"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID Client"));

    return model;
}

bool projet::supprimer(QString id_projet)
{
    QSqlQuery query;
    query.prepare("DELETE FROM projet WHERE id_projet = :id_projet");
    query.bindValue(":id_projet", id_projet);
    return query.exec();
}

bool projet::modifier(QString id_projet, QString nom, QString etat, QDate date_debut, QDate date_fin, int budget, QString id_employe,int id_client)
{
    QSqlQuery query;
    query.prepare("UPDATE projet SET nom = :nom, etat = :etat, date_debut = :date_debut, date_fin = :date_fin, budget = :budget, id_employe = :id_employe, id_client = :id_client WHERE id_projet = :id_projet");
    query.bindValue(":id_projet", id_projet);
    query.bindValue(":nom", nom);
    query.bindValue(":etat", etat);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":budget", budget);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":id_client", id_client);
    return query.exec();
}

QList<QString> projet::getEmployeeIds() {
    QList<QString> ids;
    QSqlQuery query;
    query.exec("SELECT id_employe FROM employe");
    while (query.next()) {
        ids.append(query.value(0).toString());
    }
    return ids;
}

QList<int> projet::getClientIds() {
    QList<int> ids;
    QSqlQuery query;
    query.exec("SELECT id FROM clients");
    while (query.next()) {
        int id = query.value(0).toInt();

        ids.append(id);
    }
    return ids;
}

QList<projet> projet::getProjects() {
    QList<projet> projects;
    QSqlQuery query;
    query.exec("SELECT * FROM projet");
    while (query.next()) {
        QString id_projet = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString etat = query.value(2).toString();
        QDate date_debut = query.value(3).toDate();
        QDate date_fin = query.value(4).toDate();
        int budget = query.value(5).toInt();
        QString id_employe = query.value(6).toString();
        int id_client = query.value(7).toInt();
        QByteArray qrCode = query.value(8).toByteArray(); // Assuming the QR code is stored as a BLOB
        QList<QString> taches = query.value(9).toString().split(", "); // Assuming taches is stored as comma-separated values in the database

        projet p(id_projet, nom, etat, date_debut, date_fin, budget, id_employe, id_client, qrCode, taches);
        projects.append(p);
    }
    return projects;
}

QString projet::getProjectIdFromName(const QString& projectName) {
    // Create a QSqlQuery object
    QSqlQuery query;

    // Prepare the SQL query to retrieve the project ID based on the project name
    query.prepare("SELECT id_projet FROM projet WHERE nom = :projectName");

    // Bind the project name to the query
    query.bindValue(":projectName", projectName);

    // Execute the query
    if (query.exec()) {
        // Check if any results were returned
        if (query.next()) {
            // Retrieve the project ID from the query result
            return query.value(0).toString();
        } else {
            // If no results were returned, return an empty string
            return "";
        }
    }
}
bool projet::updateTasksForProject(const QString& projectId, const QString& tacheFait, const QString& tacheEnCours, const QString& tacheAFaire) {
    QSqlQuery query;
    query.prepare("UPDATE projet SET taches = :taches WHERE id_projet = :projectId");
    query.bindValue(":taches", tacheFait + ", " + tacheEnCours + ", " + tacheAFaire);
    query.bindValue(":projectId", projectId);
    return query.exec();
}

QList<QString> projet::getTasksForProject(const QString& projectId) {
    QList<QString> tasks;
    QSqlQuery query;
    query.prepare("SELECT taches FROM projet WHERE id_projet = :projectId");
    query.bindValue(":projectId", projectId);
    if (query.exec() && query.next()) {
        QString taches = query.value(0).toString();
        tasks = taches.split(", ");
    }
    return tasks;
}
QChartView* generateProjectStateStatistics() {
    // Create a bar set to hold the data
    QBarSet *barSet = new QBarSet("Project State");

    // Initialize counters for each state
    int ongoingCount = 0;
    int completedCount = 0;
    int pendingCount = 0;

    // Retrieve projects data from the database
    QList<projet> projects = projet::getProjects();

    // Calculate the count for each state
    for (const projet& p : projects) {
        QString state = p.getEtat();
        if (state == "En cours") {
            ongoingCount++;
        } else if (state == "Terminé") {
            completedCount++;
        } else if (state == "En attente") {
            pendingCount++;
        }
    }

    // Add data to the bar set
    *barSet << ongoingCount << completedCount << pendingCount;

    // Create a bar series and add the bar set to it
    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    // Create a chart and add the bar series to it
    QChart *chart = new QChart();
    chart->addSeries(barSeries);

    // Set up the category axis for the project states
    QStringList categories;
    categories << "En cours" << "Terminé" << "En attente";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    // Create a value axis
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // Set chart title
    chart->setTitle("Project State Statistics");

    // Create a chart view and set the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void projet::exportProjectInfoToPDF(int id_projet)
{
    // Retrieve the information of the project from the database
    QSqlQuery query;
    query.prepare("SELECT nom, etat, date_debut, date_fin, budget, id_employe, id_client FROM projet WHERE id_projet = :id_projet");
    query.bindValue(":id_projet", id_projet);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des informations du projet pour l'exportation.";
        return;
    }

    if (!query.next()) {
        qDebug() << "Aucun projet trouvé avec l'ID spécifié.";
        return;
    }

    QString nom = query.value(0).toString();
    QString etat = query.value(1).toString();
    QString date_debut = query.value(2).toString();
    QString date_fin = query.value(3).toString();
    int budget = query.value(4).toInt();
    QString id_employe = query.value(5).toString();
    int id_client = query.value(6).toInt();

    // Définir le nom du fichier PDF
    QString fileName = "C:/Users/karim/OneDrive/Bureau/document/projet C++/2a2-unityforce-integration-finale/pdfprojet/projet_info_" + QString::number(id_projet) + ".pdf";

    // Créer le périphérique d'impression
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Créer un painter associé au périphérique d'impression
    QPainter painter;
    painter.begin(&printer);

    // Dessiner les informations du projet sur le painter
    painter.drawText(100, 100, "Nom: " + nom);
    painter.drawText(100, 120, "État: " + etat);
    painter.drawText(100, 140, "Budget: " + QString::number(budget));
    painter.drawText(100, 160, "Date début: " + date_debut);
    painter.drawText(100, 180, "Date fin: " + date_fin);
    painter.drawText(100, 200, "ID Employé: " + id_employe);
    painter.drawText(100, 220, "ID Client: " + QString::number(id_client));

    // Fin du dessin
    painter.end();

    qDebug() << "Informations du projet exportées avec succès vers le fichier PDF: " << fileName;
}
QSqlQueryModel* projet::sortProjectsByBudget()
{
    // Retrieve the information of projects sorted by budget from the database
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_projet, nom, etat, date_debut, date_fin, budget, id_employe, id_client FROM projet ORDER BY budget DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date fin"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Budget"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID Client"));

    // Return the updated model
    return model;
}
bool projet::fetchProjectDetailsForModification(const QString& id_projet, QString& nom, QString& etat, QDate& date_debut, QDate& date_fin, int& budget, QString& id_employe, int& id_client)
{
    // Fetch the project details from the database based on the ID
    QSqlQuery query;
    query.prepare("SELECT nom, etat, date_debut, date_fin, budget, id_employe, id_client FROM projet WHERE id_projet = :id_projet");
    query.bindValue(":id_projet", id_projet);
    if (query.exec() && query.next()) {
        nom = query.value(0).toString();
        etat = query.value(1).toString();
        date_debut = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        date_fin = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
        budget = query.value(4).toInt();
        id_employe = query.value(5).toString();
        id_client = query.value(6).toInt();
        return true;
    } else {
        return false;
    }
}
QSqlQueryModel* projet::searchProjectByName(const QString& nomProjet)
{
    // Prepare the SQL query with a WHERE clause to search by name
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id_projet, nom, etat, date_debut, date_fin, budget, id_employe, id_client FROM projet WHERE nom LIKE :nomProjet");
    query.bindValue(":nomProjet", "%" + nomProjet + "%"); // Using LIKE to find partial matches
    if (!query.exec()) {
        qDebug() << "Error executing the search query.";
        return nullptr;
    }

    // Update the view model with the search results
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date fin"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Budget"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID Client"));

    // Return the view model
    return model;
}
void projet::createStat(QStackedWidget *stackedWidget) {
    // Récupérer les données de statistiques sur les catégories d'équipements
    QSqlQuery query;
    query.exec("SELECT etat, COUNT(*) FROM projet GROUP BY etat");

    // Créer un modèle de données pour les catégories et les nombres d'équipements
    QBarSet *set = new QBarSet("Nombre de projet par etat");
    QStringList categories; // Liste pour stocker les noms des catégories
    while (query.next()) {
        QString categorie = query.value(0).toString();
        int count = query.value(1).toInt();
        *set << count;
        categories << categorie; // Ajouter le nom de la catégorie à la liste
    }

    // Créer un diagramme à barres
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques de projets par etat");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Créer un axe des catégories et définir les catégories
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories); // Ajouter les noms des catégories à l'axe des catégories
    chart->addAxis(axisX, Qt::AlignBottom); // Ajouter l'axe des catégories au graphique

    // Créer une vue graphique pour le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher le graphique dans le stacked widget dédié à la statistique d'équipement
    stackedWidget->addWidget(chartView);
    stackedWidget->setCurrentWidget(chartView);
}

