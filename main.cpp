#include <QApplication>
#include <QMainWindow>
#include <QStringList>
#include <QMessageBox>

#include "gui/CentralWidget.h"
#include "utils/utils.h"
#include "jsonWriter.h"

#include <QThread>


int main(int argc, char **argv){

    QApplication app(argc, argv);

    QMainWindow mainWindow;
    auto centralWgt = new CentralWidget(&mainWindow);
    mainWindow.setWindowTitle("ALT Linux Branches Comparison Tool");
    mainWindow.resize(800, 600);
    mainWindow.show();
    mainWindow.setCentralWidget(centralWgt);
    JSONWriter writer;

    QObject::connect(centralWgt, &CentralWidget::compareClicked, [&centralWgt, &writer](){
        auto branchNames = centralWgt->getBranchNames();
        if(branchNames.first == branchNames.second){
            QMessageBox::warning(centralWgt,"Error","Ветки должны различаться");
            return ;
        }
        auto branches = utils::compareBranches(branchNames.first, branchNames.second);
        centralWgt->onComparedDataLoaded(branches.first);
        writer.setJsObject(branches.second);
    });

    QObject::connect(centralWgt, &CentralWidget::saveClicked, [&writer](){
        writer.whrite();
    });

    auto branchNames = utils::getBranchesList();
    centralWgt->onComboboxDataLoaded(branchNames, branchNames);
    return app.exec();
}