#ifndef FILESEARCH_H
#define FILESEARCH_H

#include "includes.h"

std::string strLWR(std::string in){
  std::string res;
  res=in;
  std::transform(res.begin(), res.end(), res.begin(),
      [](unsigned char c){ return std::tolower(c); });
  return res;
};



QStringList fileSearch(QString Path, QString Extension){
    QStringList files;
    QString file;
    if (fs::exists(Path.toStdString()) && fs::is_directory(Path.toStdString())) //checks if the path is a directory and exists
    {
        for (const auto& entry : fs::directory_iterator(Path.toStdString())) //iterates objects in the directory
        {
            if (!fs::is_directory(entry.path())) //if the object isn't a directory it tries to open it with fstream
            {
                if(strLWR(entry.path().extension())==strLWR(Extension.toStdString())){
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
