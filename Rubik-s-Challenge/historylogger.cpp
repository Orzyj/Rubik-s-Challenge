#include "historylogger.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

bool HistoryLogger::exportAsCSV(std::vector<TMove> data)
{
    QString destkopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = destkopPath + "/" + "data.csv";
    QFile file(filePath);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);

    for(int i = 0; i < data.size(); i++) {
        QString id = QString::number(data[i].id);
        QString direction = QString::fromStdString(getMoveText(data[i].direction));
        QChar axis = data[i].axis;
        QString angle = QString::number(data[i].angle);

        out << id + ";" + direction + ";" + axis + ";" + angle + "\n";
    }

    return true;
}

bool HistoryLogger::exportAsJSON(std::vector<TMove> data)
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = desktopPath + "/data.json";
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QJsonArray moveArray;

    for (const auto& move : data) {
        QJsonObject moveObject;
        moveObject["id"] = move.id;
        moveObject["direction"] = QString::fromStdString(getMoveText(move.direction));
        moveObject["axis"] = QString(move.axis);
        moveObject["angle"] = move.angle;
        moveObject["selectedOption"] = move.selectedOption;

        moveArray.append(moveObject);
    }

    QJsonDocument doc(moveArray);
    file.write(doc.toJson());

    file.close();
    return true;
}
