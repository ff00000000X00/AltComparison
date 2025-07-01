#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVector>
#include <QMap>
#include <QDebug>

#include <optional>

#include "../include/libAltBranches.h"
#include "../structs/comparedBranches.h"





namespace utils {

const QVector<QString> errList = {"Failed to retrieve data", "Retrieved data is empty!", "Cant pars JSON data"};

enum Errors {
    DATA_RETRIVE_ERROR=0,
    DATA_IS_EMPTY,
    CANT_PARSE_DATA
};

    std::optional<QJsonObject> getReqData(const QString &url) {
        auto ret = make_rest_request(url.toStdString().c_str());
        if(!ret){
            qWarning()<<errList[Errors::DATA_RETRIVE_ERROR] + "from "  + url << "\n";
            return std::nullopt;
        }
        QByteArray jsonData(ret);
        if (jsonData.isEmpty()) {
            qWarning()<<errList[Errors::DATA_IS_EMPTY]<< "\n";
            return std::nullopt;
        }
        QJsonObject jsObj = QJsonDocument::fromJson(jsonData.trimmed()).object();
        if (jsObj.isEmpty()) {
            qWarning()<<errList[Errors::CANT_PARSE_DATA]<< "\n";
            return std::nullopt;
        }
        return jsObj;
    }

    QStringList getBranchesList(){
        auto data = getReqData("https://rdb.altlinux.org/api/errata/errata_branches");
        QStringList branchNames;
        if(!data){
            return {};
        }

        QJsonArray jsArr = data.value()["branches"].toArray();
        for (const auto &el : jsArr) {
            branchNames.append(el.toString());
        }
        return branchNames;
    }

    ComparedBranchesData compareBranches(const QString branch1, const QString branch2){
        const char *ret = compare_branches(branch1.toUtf8(), branch2.toUtf8());
        if(!ret){
            qWarning()<<errList[Errors::DATA_RETRIVE_ERROR] << "\n";
            return{};
        }
        QByteArray jsonData(ret);
        if (jsonData.isEmpty()) {
            qWarning()<<errList[Errors::DATA_IS_EMPTY]<< "\n";
            return {};
        }

        QJsonObject jsObj = QJsonDocument::fromJson(jsonData.trimmed()).object();
        if (jsObj.isEmpty()) {
            qWarning()<<errList[Errors::CANT_PARSE_DATA]<< "\n";
            return {};
        }

        auto getBranchData = [&](const QString& key) -> QMap<QString,QVector<QString>>{
            auto jsObjArch = jsObj[key].toObject();
            QMap<QString,QVector<QString>> resData;
            for(const auto& el: jsObjArch.keys()){

                QJsonArray arr = jsObjArch[el].toArray();
                QVector<QString> packs;
                packs.reserve(arr.size());

                for (const auto &val : arr) {
                    packs.append(val.toString());
                }

                resData.insert(el, packs);
            }

            return resData;
        };

        ComparedBranchesData cbd{getBranchData("only_in_first"), getBranchData("only_in_second"), getBranchData("newer_in_first")};

        return cbd;
    }
}