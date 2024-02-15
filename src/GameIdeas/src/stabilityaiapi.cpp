#include "include/stabilityaiapi.h"

#include <QNetworkRequest>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

StabilityAiAPI::StabilityAiAPI()
{
     nManager=new QNetworkAccessManager(this);
     connect(nManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyFinished(QNetworkReply*)));
}

void StabilityAiAPI::generateImage(QString prompt){ //https://platform.stability.ai/docs/api-reference#tag/v1generation/operation/textToImage
    QString path="https://api.stability.ai/v1/generation/stable-diffusion-v1-6/text-to-image";
    QUrl apiUrl=path;
    //Header parameters
    QString accept="image/png"; //accept image/png
    QString apiKey="sk-B2RDBkF9vUpYv1Hk1nFzvRZQc1XUXw4GlcjUQaHFpD01fVNm";//For header authorization
    //addRequest
    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",apiKey.toUtf8());
    request.setRawHeader("Accept",accept.toUtf8());
    int height=512;
    int width=512;
    //to json format
    //REQUEST BODY SCHEMA: application/json
    QJsonObject requestBody;
    requestBody["height"]=height;
    requestBody["width"]=width;
    QJsonArray textPrompts;
    QJsonObject textPrompt;
    textPrompt["text"]=prompt;
    textPrompt["weight"] = 0.5;
    textPrompts.append(textPrompt);
    requestBody["text_prompts"]=textPrompts;
    QByteArray requestData=QJsonDocument(requestBody).toJson();
    //POST request
    reply=nManager->post(request, requestData);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));
}

void StabilityAiAPI::networkReplyFinished(QNetworkReply *reply){
    if (reply->error() != QNetworkReply::NoError) {
        // Handle the error
        qDebug() << "Error:" << reply->errorString();
        qDebug() << "HTTP Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Response:" << reply->readAll();
        emit error();

    } else {
        // Process the successful response
        QPixmap image;
        QByteArray imageData = reply->readAll();
        // Create QPixmap from the image data
        image.loadFromData(imageData);
        emit gettedImage(image);
    }
    // Cleanup
    reply->deleteLater();
}

StabilityAiAPI::~StabilityAiAPI(){
    reply->close();
}
