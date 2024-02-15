#include "include/gameideas.h"
#include "ui/ui_gameideas.h"
#include "include/genres.h"
#include "include/image.h"

#include <QList>
#include <QDebug>
#include <QMessageBox>

GameIdeas::GameIdeas(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameIdeas)
{
    ui->setupUi(this);
    connect(ui->addNewPushButton, SIGNAL(clicked()), this, SLOT(slotAddNewIdea()));
    connect(ui->savePushButton, SIGNAL(clicked()), this, SLOT(slotSavePushButton()));
    connect(ui->deletePushButton, SIGNAL(clicked()), this, SLOT(slotDeletePushButton()));
    connect(ui->openPushButton, SIGNAL(clicked()), this, SLOT(slotOpenPushButton()));
    connect(ui->ideasListTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(slotOpenPushButton()));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchIdeas(QString)));
    connect(ui->gameGenresPushButton, SIGNAL(clicked()), this, SLOT(slotOpenGenresPushButton()));
    connect(ui->generateImagePushButton, SIGNAL(clicked()), this, SLOT(slotGenerateImage()));
    db.readIdeasFromDatabase(ui->ideasListTableWidget);
    currentId=-1;
}


void GameIdeas::slotSavePushButton(){
    //add id check for edit
    if (ui->gameNameLineEdit->text().isEmpty()){
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Game title must be filled!");
        messageBox.setFixedSize(500,200);
    }
    else{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDate = currentDateTime.toString("dd-MM-yyyy HH:mm:ss");
    db.saveToDatabase(currentId, ui->gameNameLineEdit->text(), ui->genresListWidget, ui->ideaPlainTextEdit->toPlainText(), currentDate, image);
    //Yes there is leak of exec time, but this fragment of code for easy maintenance
    db.addSavedToTable(currentId, ui->gameNameLineEdit->text(), currentDate, ui->ideasListTableWidget);
    slotAddNewIdea();
    }
}

void GameIdeas::slotDeletePushButton(){
    QList<QTableWidgetSelectionRange> selectedList=ui->ideasListTableWidget->selectedRanges();
    QMessageBox::StandardButton reply;
       reply = QMessageBox::question(nullptr, "Confirmation", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
       if (reply == QMessageBox::No) {
           return;
       } else {
           db.deleteFromDatabase(ui->ideasListTableWidget, selectedList);
           currentId=-1;
           ui->gameNameLineEdit->clear();
           ui->genresListWidget->clear();
           ui->ideaPlainTextEdit->clear();
       }
}

void GameIdeas::slotOpenPushButton(){
    QList<QTableWidgetItem *> selectedItems = ui->ideasListTableWidget->selectedItems();
    if (selectedItems.size() > 1) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Select only one idea!");
        messageBox.setFixedSize(500,200);
    }
    else{
        if (!(selectedItems.size() == 1)){
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Select one idea!");
            messageBox.setFixedSize(500,200);
        }
        else{
            QString id = ui->ideasListTableWidget->item(selectedItems[0]->row(), 0)->text();
            currentId = id.toInt();
            db.showSelectedIdea(id, ui->gameNameLineEdit, ui->genresListWidget, ui->ideaPlainTextEdit, &image);
        }
    }
}

void GameIdeas::slotSearchIdeas(const QString &changedText){
    for (int i=0; i<ui->ideasListTableWidget->rowCount();++i){
        QTableWidgetItem *item=ui->ideasListTableWidget->item(i,1);
        QString itemText = item->text();
        if (itemText.contains(changedText, Qt::CaseInsensitive)){
            ui->ideasListTableWidget->setRowHidden(i, false);
        }
        else{
            ui->ideasListTableWidget->setRowHidden(i, true);
        }
    }
}

void GameIdeas::slotOpenGenresPushButton(){
    Genres *gameGenresWindow=new Genres(ui, this);
    gameGenresWindow->setModal(1);
    gameGenresWindow->show();
}

void GameIdeas::slotAddNewIdea(){
    image=QPixmap();
    currentId = -1;
    ui->gameNameLineEdit->clear();
    ui->genresListWidget->clear();
    ui->ideaPlainTextEdit->clear();
}

void GameIdeas::slotGenerateImage(){
    Image *imageWindow=new Image(ui->ideaPlainTextEdit->toPlainText(), this, image);
    imageWindow->setModal(1);
    imageWindow->show();
}

GameIdeas::~GameIdeas()
{
    delete ui;
}
