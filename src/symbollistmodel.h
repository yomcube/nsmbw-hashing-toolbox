#ifndef SYMBOLLISTMODEL_H
#define SYMBOLLISTMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <vector>

typedef struct {
    bool isCracked;
    QString hashNameMang;
    QString hashNameDemNV;
    QString hashNameDem;
    QString comment;
    uint32_t addr;
    QDateTime timeAdded;
} SymbolInfo;

class SymbolListModel : public QAbstractTableModel {
    Q_OBJECT
public:
    SymbolListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void loadData(QString csv);

private:
    std::vector<SymbolInfo> syms;

};

#endif // SYMBOLLISTMODEL_H
