#include <QWidget>

#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

class QComboBox;
class QTreeView;
class TreeModel;
class ComparedBranchesData;

class CentralWidget : public QWidget {
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget() override;
    QPair<QString, QString> getBranchNames();
  signals:
    void compareClicked();
public slots:
    void onComparedDataLoaded(const ComparedBranchesData& modelData);
    void onComboboxDataLoaded(const QStringList &leftData, const QStringList &rightData);
private:
    QComboBox *m_leftCombo = nullptr;
    QComboBox *m_rightCombo = nullptr;
    QTreeView *m_dataTree = nullptr;
    TreeModel *m_treeModel = nullptr;

};

#endif // CENTRALWIDGET_H