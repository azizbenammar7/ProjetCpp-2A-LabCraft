#ifndef CUSTOMSQLMODEL_H
#define CUSTOMSQLMODEL_H

#include <QSqlQueryModel>
#include <QPixmap>
#include <QVariant>

class CustomSqlModel : public QSqlQueryModel
{
public:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // CUSTOMSQLMODEL_H
