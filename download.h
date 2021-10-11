#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "includes.h"
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QStringList>
#include <QTimer>
#include <QUrl>

#include <stdio.h>


class QSslError;

class download: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;

private:
    QStringList URLs;
    QString basePath;
    QString filename;

public:
    download();
    void setURLs(QStringList args);
    void setPath(QString arg);
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    void saveFileName(QString basename);
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
};

#endif // DOWNLOAD_H
