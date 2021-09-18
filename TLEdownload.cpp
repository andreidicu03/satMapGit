#include <QCoreApplication>

#include "TLEdownload.h"

// constructor
TLEdownload::TLEdownload()
{
    // signal finish(), calls downloadFinished()
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(downloadFinished(QNetworkReply*)));
}

void TLEdownload::execute()
{
    // get the argument including program's name
    QStringList args = QCoreApplication::instance()->arguments();

    // skip the first argument, which is the program's name
    args.takeFirst();



    // process each url starting from the 2nd one
    foreach (QString arg, args) {

        // QString::toLocal8Bit()
        //  - local 8-bit representation of the string as a QByteArray
        // Qurl::fromEncoded(QByteArray)
        //  - Parses input and returns the corresponding QUrl.
        //    input is assumed to be in encoded form,
        //    containing only ASCII characters.

        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());

        // makes a request
        doDownload(url);
    }
}

// Constructs a QList of QNetworkReply
void TLEdownload::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif

    // List of reply
    currentDownloads.append(reply);
}

QString TLEdownload::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "TLE";

    if (QFile::exists(basename)) {
        QFile::remove(basename);
    }

    return basename;
}

void TLEdownload::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply))
            printf("Download of %s succeeded (saved to %s)\n",
                   url.toEncoded().constData(), qPrintable(filename));
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

}

bool TLEdownload::saveToDisk(const QString &filename, QIODevice *reply)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(reply->readAll());
    file.close();

    return true;
}

void TLEdownload::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_SSL
    foreach (const QSslError &error, sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

