#include "include/databaseoperations.h"

#include <QDir>
#include <QDateTime>
#include <QHeaderView>
#include <QMessageBox>
#include <QPixmap>
#include <QBuffer>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonArray>
#include <QJsonDocument>

//Module of database management
DatabaseOperations::DatabaseOperations(){
    localDB = QSqlDatabase::addDatabase("QSQLITE");
    localDB.setHostName("localhost");
    localDB.setDatabaseName(QDir::currentPath()+"/DataBase.sqlite");
    localDB.setUserName("");
    localDB.setPassword("");
    qDebug()<<QSqlDatabase::drivers();
    if (!localDB.open()){
        qDebug()<<"database not connected\n";
        qDebug()<<localDB.lastError().text();
    }
    QSqlQuery qr;
    if (!qr.exec("CREATE TABLE IF NOT EXISTS localIdeas("
                 "Name TEXT, "
                 "Genres TEXT, "
                 "TextOfIdea TEXT, "
                 "LastModifiedDate TEXT,"
                 "Image BLOB)"))
    {
        qDebug()<<"Не удалось создать таблицу";
        qDebug()<<localDB.lastError().text();
    }
}


void DatabaseOperations::saveToDatabase(int &idOfIdea, QString nameOfGame, QListWidget *genresOfGame, QString textOfGameIdea, QString currentDate, QPixmap image){
    QJsonArray jsonArray;
    for(int i = 0; i < genresOfGame->count(); ++i) {
        jsonArray.append((genresOfGame->item(i))->text());
    }
    QJsonDocument jsonDocument(jsonArray);
    QString jsonGenresData=jsonDocument.toJson();
    QSqlQuery query;
    //Convert QPixmap into inByteArray
    QByteArray inByteArray;                             // Создаём объект QByteArray для сохранения изображения
    QBuffer inBuffer( &inByteArray );                   // Сохранение изображения производим через буффер
    inBuffer.open( QIODevice::WriteOnly );              // Открываем буффер
    image.save( &inBuffer, "PNG" );
    //Check if id -1 or another
    if (idOfIdea==-1){
        //Add new idea to database
        //Get listWidget to Json, then initialaze adding to Database
        query.prepare("INSERT INTO localIdeas (Name, Genres, TextOfIdea, LastModifiedDate, Image) "
                      "VALUES (:Name, :Genres, :TextOfIdea, :LastModifiedDate, :Image)");
        query.bindValue(":Name", nameOfGame);
        query.bindValue(":Genres", jsonGenresData);
        query.bindValue(":TextOfIdea", textOfGameIdea);
        query.bindValue(":LastModifiedDate", currentDate);
        query.bindValue(":Image", inByteArray);
        if (!query.exec()){
            qDebug()<<query.lastError().text();
        }
        idOfIdea=query.lastInsertId().toInt();
    }
    else{
        //Edit existing idea in database
        query.prepare("SELECT rowid, Name, Genres, TextOfIdea, LastModifiedDate, Image FROM localIdeas WHERE rowid=:rowid");
        query.bindValue(":rowid", idOfIdea);
        if (!query.exec()){
            qDebug()<<query.lastError().text();
        }
        else{
            if (!query.next()){
                qDebug()<<query.lastError().text();
                QMessageBox messageBox;
                messageBox.critical(0,"Error","Something went wrong");
                messageBox.setFixedSize(500,200);
            }
            else{
                query.prepare("UPDATE localIdeas SET Name=:Name, Genres=:Genres, TextOfIdea=:TextOfIdea, LastModifiedDate=:LastModifiedDate WHERE rowid=:rowid");
                query.bindValue(":rowid", idOfIdea);
                query.bindValue(":Name", nameOfGame);
                query.bindValue(":Genres", jsonGenresData);
                query.bindValue(":TextOfIdea", textOfGameIdea);
                query.bindValue(":LastModifiedDate", currentDate);
                query.bindValue(":Image", inByteArray);
                if (!query.exec()){
                    qDebug()<<query.lastError().text();
                }
            }
        }
    }
}

void DatabaseOperations::readIdeasFromDatabase(QTableWidget *tableOfIdeas){
    QSqlQuery query;
    if (!query.exec("SELECT rowid, Name, Genres, TextOfIdea, LastModifiedDate, Image FROM localIdeas")) {
        qDebug()<<query.lastError().text();
    }
    else{
        //Leak of exec time, but easy maintenance
        tableOfIdeas->setRowCount(0);
        for(int i=0; query.next(); i++){
            QString id = query.value(0).toString();
            QString name = query.value(1).toString();
            QString currentDate=query.value(4).toString();
            tableOfIdeas->insertRow(i);
            tableOfIdeas->setItem(i, 0, new QTableWidgetItem(id));
            tableOfIdeas->setItem(i, 1, new QTableWidgetItem(name));
            tableOfIdeas->setItem(i, 2, new QTableWidgetItem(currentDate));
        }
        tableOfIdeas->setColumnHidden(0, true);
        tableOfIdeas->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tableOfIdeas->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        tableOfIdeas->setColumnWidth(2,220);
    }
}

void DatabaseOperations::deleteFromDatabase(QTableWidget *tableOfIdeas, QList<QTableWidgetSelectionRange> selectedList){
    for (const QTableWidgetSelectionRange &range : selectedList) {
        int topRow = range.topRow();
        int bottomRow = range.bottomRow();
        for (int row = bottomRow; row >= topRow; --row) {
            QSqlQuery query;
            query.prepare("DELETE FROM localIdeas WHERE rowid = :rowid");
            QString id=tableOfIdeas->item(row, 0)->text();
            query.bindValue(":rowid", id);
            if (!query.exec()) {
                qDebug()<<query.lastError().text();
            }
            tableOfIdeas->removeRow(row);
        }
    }
}

void DatabaseOperations::showSelectedIdea(QString selectedItemId, QLineEdit *gameTitle, QListWidget *genresOfGame, QPlainTextEdit *textOfGameIdea, QPixmap *image){
    QSqlQuery query;
    query.prepare("SELECT rowid, Name, Genres, TextOfIdea, LastModifiedDate, Image FROM localIdeas WHERE rowid=:rowid");
    query.bindValue(":rowid", selectedItemId);
    if (!query.exec()) {
        qDebug()<<query.lastError().text();
    }
    else{
        //Leak of exec time, but easy maintenance
        while (query.next()){
            gameTitle->setText(query.value(1).toString());
            genresOfGame->clear();
            QString genresFromDataBase=query.value(2).toString();
            QJsonDocument jsonDocument = QJsonDocument::fromJson(genresFromDataBase.toUtf8());
            QJsonArray jsonArray = jsonDocument.array();
            for(int i=0; i<jsonArray.size(); ++i){
                genresOfGame->addItem(new QListWidgetItem(jsonArray[i].toString()));
            }
            textOfGameIdea->setPlainText(query.value(3).toString());
            image->loadFromData(query.value(5).toByteArray());
        }
    }
}

void DatabaseOperations::addSavedToTable(int &idOfIdea, QString nameOfGame, QString currentDate, QTableWidget *tableOfIdeas){
    QSqlQuery query;
    query.prepare("SELECT rowid, Name, Genres, TextOfIdea, LastModifiedDate, Image FROM localIdeas WHERE rowid=:rowid");
    query.bindValue(":rowid", idOfIdea);
    if (!query.exec()) {
        qDebug()<<query.lastError().text();
    }
    else{
        //Leak of exec time, but easy maintenance
        while (query.next()){
            QString id = query.value(0).toString();
            QString name = query.value(1).toString();
            QString currentDate=query.value(4).toString();
            int newRow=tableOfIdeas->rowCount();
            //disableSorting for correct display
            tableOfIdeas->setSortingEnabled(false);
            tableOfIdeas->insertRow(newRow);
            tableOfIdeas->setItem(newRow, 0, new QTableWidgetItem(id));
            tableOfIdeas->setItem(newRow, 1, new QTableWidgetItem(name));
            tableOfIdeas->setItem(newRow, 2, new QTableWidgetItem(currentDate));
            tableOfIdeas->setSortingEnabled(true);
        }
    }
}
