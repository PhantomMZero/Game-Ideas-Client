#include "include/genres.h"
#include "ui/ui_genres.h"

#include <QMessageBox>

Genres::Genres(Ui::GameIdeas *gameIdeasUi, GameIdeas *gameIdeasObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Genres)
{
    ui->setupUi(this);
    Genres::gameIdeasObject=gameIdeasObject;
    Genres::gameIdeasUi=gameIdeasUi;
    addGenresFromGameIdeasUi();
    connect(ui->addSelectedGenrePushButton, SIGNAL(clicked()), this, SLOT(slotAddSelectedGenre()));
    connect(ui->genresListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotAddSelectedGenre()));
    connect(ui->deleteSelectedPushButton, SIGNAL(clicked()), this, SLOT(slotDeleteSelectedGenres()));
    connect(ui->addCustomGenrePushButton, SIGNAL(clicked()), this, SLOT(slotAddCustomGenre()));
    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchCustomGenre(QString)));
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
    connect(ui->confirmPushButton, SIGNAL(clicked()), this, SLOT(slotConfirm()));
}

Genres::~Genres()
{
    delete ui;
}

void Genres::addGenresFromGameIdeasUi(){
    for(int i = 0; i < gameIdeasUi->genresListWidget->count(); ++i)
    {
        QListWidgetItem *item = gameIdeasUi->genresListWidget->item(i);
        ui->myGenresListWidget->addItem(item->text());
    }
}

void Genres::slotAddSelectedGenre(){
    QList<QListWidgetItem*> items = ui->genresListWidget->selectedItems();
    //Selection mode Single
    ui->myGenresListWidget->addItem(items[0]->text());
}

void Genres::slotDeleteSelectedGenres(){
    QList<QListWidgetItem*> items = ui->myGenresListWidget->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        delete ui->myGenresListWidget->takeItem(ui->myGenresListWidget->row(item));
    }
}

void Genres::slotAddCustomGenre(){
    if (ui->customGenreLineEdit->text().isEmpty()){
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Custom genre must be filled!");
        messageBox.setFixedSize(500,200);
    }
    else{
        ui->myGenresListWidget->addItem(ui->customGenreLineEdit->text());
        ui->customGenreLineEdit->clear();
    }
}

void Genres::slotSearchCustomGenre(const QString &changedText){
    for (int i=0; i<ui->genresListWidget->count();++i){
        QString currentGenreInRow=ui->genresListWidget->item(i)->text();
        if (currentGenreInRow.contains(changedText, Qt::CaseInsensitive)){
            ui->genresListWidget->item(i)->setHidden(false);
        }
        else{
            ui->genresListWidget->item(i)->setHidden(true);
        }
    }
}

void Genres::slotConfirm(){
    gameIdeasUi->genresListWidget->clear();
    for(int i = 0; i < ui->myGenresListWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->myGenresListWidget->item(i);
        gameIdeasUi->genresListWidget->addItem(item->text());
    }
    gameIdeasObject->show();
    this->close();
}

void Genres::slotCancel(){
    gameIdeasObject->show();
    this->close();
}
