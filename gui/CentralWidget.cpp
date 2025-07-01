#include <QBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QTreeView>
#include <QStringListModel>
#include <QHeaderView>

#include "CentralWidget.h"
#include "TreeModel.h"



CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent) 
    , m_leftCombo(new QComboBox(this))
    , m_rightCombo(new QComboBox(this))
    , m_dataTree (new QTreeView(this))
    , m_treeModel(new TreeModel(this))
    {
    
    m_dataTree->setRootIsDecorated(true);
    m_dataTree->setItemsExpandable(true);
    auto button = new QPushButton("compare", this); 
    m_dataTree->setModel(m_treeModel);

    m_dataTree->header()->resizeSection(1, 100);
    m_dataTree->header()->resizeSection(1, 200);
    m_dataTree->header()->resizeSection(2, 200);
    m_dataTree->header()->resizeSection(3, 200);
    m_dataTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_dataTree->header()->setSectionResizeMode(0,QHeaderView::Fixed);

    QHBoxLayout *comboLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(20);
    mainLayout->addLayout(comboLayout);
    mainLayout->addWidget(m_dataTree);

    comboLayout->addWidget(m_leftCombo);
    comboLayout->addWidget(button);
    comboLayout->addWidget(m_rightCombo);

    connect(button, &QPushButton::clicked, this, [this]() {
        emit compareClicked();
    });
}
CentralWidget::~CentralWidget() {
    
}

void CentralWidget::onComparedDataLoaded(const ComparedBranchesData& modelData){
    m_treeModel->setDataModel(modelData);
}
void CentralWidget::onComboboxDataLoaded(const QStringList &leftData, const QStringList &rightData){
    m_leftCombo->addItems(leftData);
    m_rightCombo->addItems(rightData);
}
QPair<QString, QString> CentralWidget::getBranchNames() {
    return {m_leftCombo->currentText(), m_rightCombo->currentText()};
};