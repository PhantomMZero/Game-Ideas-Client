#pragma once

#include "include/databaseoperations.h"

#include <QMainWindow>

namespace Ui {
class GameIdeas;
}

class GameIdeas : public QMainWindow
{
    Q_OBJECT

public:
    DatabaseOperations db;
    QPixmap image;
    explicit GameIdeas(QWidget *parent = nullptr);
    ~GameIdeas();
private slots:
    void slotSavePushButton();
    void slotDeletePushButton();
    void slotOpenPushButton();
    void slotOpenGenresPushButton();
    void slotAddNewIdea();
    void slotGenerateImage();
    void slotSearchIdeas(const QString &changedText);
private:
    Ui::GameIdeas *ui;
    int currentId;//For new ideas -1, for selecteted currentId=RowId
};


