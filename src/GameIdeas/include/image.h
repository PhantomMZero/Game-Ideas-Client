#pragma once

#include "include/stabilityaiapi.h"
#include "include/gameideas.h"

#include <QDialog>

namespace Ui {
class Image;
}

class Image : public QDialog
{
    Q_OBJECT

public:
    explicit Image(QString prompt, GameIdeas *gameIdeasObject, QPixmap image, QWidget *parent = nullptr);
    ~Image();

private slots:
    void slotGenerateNewButton();
    void slotConfirmButton();
    void slotCancelButton();
    void slotError();
    void slotDownloadProgress(qint64 nReceived, qint64 nTotal);
    void slotGetImage(QPixmap image);
private:
    QPixmap image;
    GameIdeas *gameIdeasObject;
    StabilityAiAPI *sAiApi;
    QString textToGenerate;
    Ui::Image *ui;
};
