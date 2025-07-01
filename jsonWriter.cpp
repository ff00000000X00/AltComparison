#include <QStandardPaths>
#include <QJsonDocument>
#include <QDir>
#include <QFile>

#include "jsonWriter.h"

bool JSONWriter::whrite(const QString & path){
    QString filePath;
    if(path.isEmpty()){
        QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        filePath = homePath + "/mydata.json";
    } else {
        filePath = path;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Не удалось открыть файл для записи");
        return false;
    }

    QJsonDocument doc(m_obj);

    file.write(doc.toJson(QJsonDocument::Indented));

    file.close();
    
    return true;
}

void JSONWriter::setJsObject(const QJsonObject & js){
    m_obj = js;
}