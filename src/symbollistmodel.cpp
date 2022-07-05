#include "symbollistmodel.h"
#include <QDebug>

SymbolListModel::SymbolListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int SymbolListModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(syms.size());
}

int SymbolListModel::columnCount(const QModelIndex &) const {
    return 6;
}

QVariant SymbolListModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole)
        switch (index.column()) {
        case 0:
            return syms[index.row()].hashNameMang;
        case 1:
            return syms[index.row()].hashNameDemNV;
        case 2:
            return syms[index.row()].hashNameDem;
        case 3:
            return "0x" + QString::number(syms[index.row()].addr, 16);
        case 4:
            return syms[index.row()].timeAdded;
        default:
            return "";
        }

    return QVariant();
}

QVariant SymbolListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    const QString headerNames[] {
        "Mangled",
        "Demangled NVIDIA",
        "Demangled",
        "Address (CHN)",
        "Time added",
        "Comment"
    };
    if (role == Qt::DisplayRole && section <= 5) {
        return headerNames[section];
    }
    return QVariant();
}

void SymbolListModel::loadData(QString csv) {
    QStringList rows = csv.split("\n");
    for (QString &row : rows) {
        QRegExp rx("\"[^\"]*\"|[^,]+");
        int pos = 0;
        QStringList fields;
        while ((pos = rx.indexIn(row, pos)) != -1) {
            fields.append(rx.cap(0));
            pos += rx.matchedLength();
        }
        SymbolInfo inf;
        inf.isCracked = true;
        inf.hashNameMang = fields[0].mid(1, fields[0].length() - 2);
        inf.hashNameDemNV = fields[1].mid(1, fields[1].length() - 2);
        inf.hashNameDem = fields[2].mid(1, fields[2].length() - 2);
        inf.addr = fields[3].toUInt(nullptr, 16);
        inf.timeAdded = QDateTime::fromSecsSinceEpoch(fields[4].toUInt());
        syms.push_back(inf);
    }
    emit layoutChanged();
}
