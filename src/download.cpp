#include <QCoreApplication>
#include <QMessageBox>

#include "download.hpp"

// constructor
download::download()
{
    // signal finish(), calls downloadFinished()
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(downloadFinished(QNetworkReply*)));
}

void download::setURLs(QStringList args)
{
    this->URLs=args;
}

void download::setPath(QString args)
{
    this->basePath=args;
}

void download::execute()
{
     // process each url starting from the 2nd one
    foreach (QString arg, URLs) {
        // QString::toLocal8Bit()
        //  - local 8-bit representation of the string as a QByteArray
        // Qurl::fromEncoded(QByteArray)
        //  - Parses input and returns the corresponding QUrl.
        //    input is assumed to be in encoded form,
        //    containing only ASCII characters.
        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
        doDownload(url);
    }
}

// Constructs a QList of QNetworkReply
void download::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif

    // List of reply
    currentDownloads.append(reply);
}

QString download::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString saveFolder=basePath;
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "unknown";

    if (QFile::exists(basename)) {
        QFile::remove(basename);
    }

    saveFolder.append("/");
    saveFolder.append(basename);

    return saveFolder;
}

void download::saveFileName(QString basename)
{

    if (basename.isEmpty())
        basename = "unknown";

    if (QFile::exists(basename)) {
        QFile::remove(basename);
    }

    filename=basename;
}

void download::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if(this->filename.isEmpty())
            this->filename = saveFileName(url);
        if (saveToDisk(filename, reply)){
            printf("Download of %s succeeded (saved to %s)\n",
                   url.toEncoded().constData(), qPrintable(filename));
            //QMessageBox about;
            //filename+=" finished downloading";
            //about.setText(filename);
            //about.exec();
            filename.clear();
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

}

bool download::saveToDisk(const QString &filename, QIODevice *reply)
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

void download::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_SSL
    foreach (const QSslError &error, sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

