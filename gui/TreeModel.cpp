
#include <QSet>

#include "TreeModel.h"
#include "../structs/comparedBranches.h"

TreeModel::TreeModel(QObject* parent)
    : QAbstractItemModel(parent) {
    }

int TreeModel::rowCount(const QModelIndex& parent) const {
    int parentId = parent.isValid() ? static_cast<int>(parent.internalId()) : 0;

    int count = 0;
    for(int i = parentId+1; i < m_nodes.size(); ++i){
        if (m_nodes[i].parentIndex == parentId)
            ++count;
    }
    return count;
}

int TreeModel::columnCount(const QModelIndex&) const {
    return 4;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const Node& node = m_nodes[static_cast<int>(index.internalId())];

    if (node.level == 1 && index.column() == 0)
        return node.data[0];

    if (node.level == 2 && index.column() >= 1 && index.column() < 4)
        return node.data[index.column()-1];

    return {};
}

QModelIndex TreeModel::index (int row, int column, const QModelIndex &parent) const{

    if (!hasIndex(row, column, parent))
        return {};

    int parentId = parent.isValid() ? static_cast<int>(parent.internalId()) : 0;
    int count = 0;
    for (int i = parentId+1; i < m_nodes.size(); ++i) {
        if (m_nodes[i].parentIndex == parentId) {
            if (count == row)
                return createIndex(row, column, quintptr(i));
            ++count;
        }
    }
    return {};
}
QModelIndex TreeModel::parent(const QModelIndex &child) const{

    if (!child.isValid())
    return {};

    int childId = static_cast<int>(child.internalId());
    int parentId = m_nodes[childId].parentIndex;

    if (parentId <= 0)
        return {};


    int grandParentId = m_nodes[parentId].parentIndex;
    if (grandParentId < 0)
        return {};

    return createIndex(parentId - (grandParentId + 1), 0, quintptr(parentId));
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Arch";
            case 1: return "Only in first";
            case 2: return "Only in second";
            case 3: return "Newer in first";
        }
    } 
    return {};
}

void TreeModel::setDataModel(const ComparedBranchesData& data) {

    beginResetModel();
    m_nodes.clear();
    m_nodes.append({0, 0, {"root"}, -1});  

    QSet<QString> allArchitectures;
    allArchitectures.unite(data.onlyInFirst.keys().toSet());
    allArchitectures.unite(data.onlyInSecond.keys().toSet());
    allArchitectures.unite(data.newerInFirst.keys().toSet());

    for (const QString& arch : allArchitectures) {
        int archIndex = m_nodes.size();

        m_nodes.append({1, archIndex, {arch}, 0});

        const QVector<QString>& onlyFirst  = data.onlyInFirst.value(arch);
        const QVector<QString>& onlySecond = data.onlyInSecond.value(arch);
        const QVector<QString>& newerFirst = data.newerInFirst.value(arch);

        int maxCount = std::max({onlyFirst.size(), onlySecond.size(), newerFirst.size()});

        for (int i = 0; i < maxCount; ++i) {
            QVector<QString> row = {
                i < onlyFirst.size() ? onlyFirst[i] : "",
                i < onlySecond.size() ? onlySecond[i] : "",
                i < newerFirst.size() ? newerFirst[i] : ""
            };

            m_nodes.append({2, m_nodes.size(), row, archIndex});
        }
    }

    endResetModel();
}
