#ifndef FILESEARCH_H
#define FILESEARCH_H

#include "includes.h"

QStringList fileSearch(QString Path, QString Extension){
    QStringList files;
    QString file;
    if (fs::exists(Path.toStdString()) && fs::is_directory(Path.toStdString())) //checks if the path is a directory and exists
    {
        for (const auto& entry : fs::directory_iterator(Path.toStdString())) //iterates objects in the directory
        {
            if (!fs::is_directory(entry.path())) //if the object isn't a directory it tries to open it with fstream
            {
                if(entry.path().extension()==Extension.toStdString()){
                    file=QString::fromStdString(entry.path().string());
                    files.append(file);
                    //qDebug()<<file;
                }
            }
        }
    }
    return files;
}
#endif // FILESEARCH_H
