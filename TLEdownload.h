#ifndef TLEDOWNLOAD_H
#define TLEDOWNLOAD_H

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

class TLEdownload: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;

private:
    QStringList URLs;

public:
    TLEdownload();
    void setURLs(QStringList args);
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
};

#endif // TLEDOWNLOAD_H
