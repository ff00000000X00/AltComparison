#include <QMap>
#include <QVector>

 using branchData = QMap<QString,QVector<QString>>;

struct ComparedBranchesData{
    branchData onlyInFirst;
    branchData onlyInSecond;
    branchData newerInFirst;
};