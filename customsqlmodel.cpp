#include "customsqlmodel.h"

QVariant CustomSqlModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 8 && role == Qt::DecorationRole) {
        QByteArray imageData = QSqlQueryModel::data(this->index(index.row(), 8), Qt::DisplayRole).toByteArray();
        if (!imageData.isEmpty()) {
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            return pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
    return QSqlQueryModel::data(index, role);
}
