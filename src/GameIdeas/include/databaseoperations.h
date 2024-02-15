#pragma once

#include <QSqlDatabase>
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPlainTextEdit>

class DatabaseOperations
{
public:
    DatabaseOperations();
    void saveToDatabase(int &idOfIdea, QString nameOfGame, QListWidget *genresOfGame, QString textOfGameIdea, QString currentDate, QPixmap image); //Method for save idea
    void deleteFromDatabase(QTableWidget *tableOfIdeas, QList<QTableWidgetSelectionRange> selectedList); //Method for delete idea
    void readIdeasFromDatabase(QTableWidget *tableOfIdeas); //Method for update from database
    void showSelectedIdea(QString selectedItemId, QLineEdit *gameTitle, QListWidget *genresOfGame, QPlainTextEdit *textOfGameIdea, QPixmap *image); //Method for showing selected idea from database
    void addSavedToTable(int &idOfIdea, QString nameOfGame, QString currentDate, QTableWidget *tableOfIdeas);
private:
    QSqlDatabase localDB;
};
