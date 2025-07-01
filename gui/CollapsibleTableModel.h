#pragma once

#include <QAbstractItemModel>
#include <QMap>
#include <QVector>
#include <QString>
#include <QSet>


struct ComparedBranchesData;
struct Node {
    int level = 0;
    int index = 0;
    QVector<QString> data;
    int parentIndex = -1;
};

struct Group {
    QString groupName;
    QVector<QVector<QString>> rows; // QVector<QString> - cols, size = 3
};

class CollapsibleGroupModel : public QAbstractItemModel {
    Q_OBJECT
public:
    CollapsibleGroupModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
    QVariant headerData(int section, Qt::Orientation, int role) const override;
    void setDataModel(const ComparedBranchesData& data);
    QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

private:
    QVector<Node> m_nodes;
};
