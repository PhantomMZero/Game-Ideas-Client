#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QPixmap>

class StabilityAiAPI :public QObject
{
    Q_OBJECT
public:
    StabilityAiAPI();
    void generateImage(QString prompt);
    ~StabilityAiAPI();
signals:
    void error();
    void gettedImage(QPixmap);
    void downloadProgress(qint64,qint64);
private slots:
    void networkReplyFinished(QNetworkReply* reply);
private:
    QNetworkAccessManager *nManager;
    QNetworkReply *reply;
};
