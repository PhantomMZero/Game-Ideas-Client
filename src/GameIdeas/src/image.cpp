#include "include/image.h"
#include "ui/ui_image.h"

#include <QMessageBox>
#include <QCloseEvent>

Image::Image(QString prompt, GameIdeas *gameIdeasObject, QPixmap image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Image)
{
    ui->setupUi(this);
    sAiApi=new StabilityAiAPI();
    Image::gameIdeasObject=gameIdeasObject;
    ui->imageLabel->setPixmap(image);
    ui->promptPlainTextEdit->setPlainText(prompt);
    connect(ui->generateNewImagePushButton, SIGNAL (clicked()), this, SLOT(slotGenerateNewButton()));
    connect(ui->confirmPushButton, SIGNAL (clicked()), this, SLOT(slotConfirmButton()));
    connect(ui->cancelPushButton, SIGNAL (clicked()), this, SLOT(slotCancelButton()));
    connect(sAiApi, SIGNAL(gettedImage(QPixmap)), this, SLOT(slotGetImage(QPixmap)));
    connect(sAiApi, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));
    connect(sAiApi, SIGNAL(error()), this, SLOT(slotError()));
}

Image::~Image()
{
    delete ui;
}

void Image::slotGenerateNewButton(){
    ui->progressBar->setValue(0);
    sAiApi->generateImage(ui->promptPlainTextEdit->toPlainText());
    this->setDisabled(true);
}

void Image::slotConfirmButton(){
    gameIdeasObject->image=image;
    gameIdeasObject->show();
    this->close();
}

void Image::slotCancelButton(){
    //gameIdeasObject->image=QPixmap();
    gameIdeasObject->show();
    this->close();
}

void Image::slotGetImage(QPixmap image){
    this->setDisabled(false);
    this->image=image;
    ui->imageLabel->setPixmap(image);
}

void Image::slotDownloadProgress(qint64 nReceived, qint64 nTotal){
    if(nTotal <= 0){
        //slotError();
        return;
    }
    ui->progressBar->setValue(100*nReceived/nTotal);
}

void Image::slotError(){
    this->setDisabled(false);
    QMessageBox messageBox;
    messageBox.critical(0, "Error", "Something went wrong!");
    messageBox.setFixedSize(500,200);
}
