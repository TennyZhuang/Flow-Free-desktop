#include "gamemodel.h"
#include "gamepoint.h"
#include "gameroute.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

GameModel* GameModel::_instance = nullptr;

GameModel::GameModel() {
    read();
}

GameModel::~GameModel() {
    if (_instance) {
        delete _instance;
    }
}

GameModel* GameModel::instance() {
    if (!_instance) {
        _instance = new GameModel;
    }

    return _instance;
}

quint32 GameModel::size() const {
    return levelSize;
}

const Level& GameModel::getLevel(quint32 levelId) const {
    if (levelId > 0 && levelId <= size()) {
        return levels[levelId - 1];
    }
}

bool GameModel::read() {
    QFile dataFile(":/data/levels.json");

    if (!dataFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file");
        return false;
    }

    QJsonArray levelsJson = QJsonDocument::fromJson(dataFile.readAll()).array();
    levelSize = levelsJson.size();

    for (QJsonArray::const_iterator levelsIter = levelsJson.begin();
            levelsIter != levelsJson.end();
            levelsIter++) {
        Level tempLevel;
        tempLevel.id = levelsIter->toObject()["id"].toInt();
        tempLevel.size = levelsIter->toObject()["size"].toInt();
        tempLevel.colorsSize = levelsIter->toObject()["colors"].toInt();
        tempLevel.points.resize(tempLevel.size);

        const auto& levelDataJsonArray = levelsIter->toObject()["data"].toArray();

        for (quint32 i = 0; i < tempLevel.size; i++) {
            for (quint32 j = 0; j < tempLevel.size; j++) {
                tempLevel.points[i].push_back(GamePoint(j, i, (Color)levelDataJsonArray[i].toArray()[j].toInt()));
            }
        }

        levels.push_back(tempLevel);
    }

    return true;
}
