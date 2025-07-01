#include <QApplication>
#include <QMainWindow>
#include <QStringList>
#include <QMessageBox>

#include "gui/CentralWidget.h"
#include "utils/utils.h"

#include <QThread>


int main(int argc, char **argv){

    QApplication app(argc, argv);

    QMainWindow mainWindow;
    auto centralWgt = new CentralWidget(&mainWindow);
    mainWindow.setWindowTitle("ALT Linux Branches Comparison Tool");
    mainWindow.resize(800, 600);
    mainWindow.show();
    mainWindow.setCentralWidget(centralWgt);

    QObject::connect(centralWgt, &CentralWidget::compareClicked, [&centralWgt](){
        auto branchNames = centralWgt->getBranchNames();
        if(branchNames.first == branchNames.second){
            QMessageBox::warning(centralWgt,"Error","Ветки должны различаться");
            return ;
        }
        auto branches = utils::compareBranches(branchNames.first, branchNames.second);
        centralWgt->onComparedDataLoaded(branches);
    });

    auto branchNames = utils::getBranchesList();
    centralWgt->onComboboxDataLoaded(branchNames, branchNames);
    return app.exec();
}