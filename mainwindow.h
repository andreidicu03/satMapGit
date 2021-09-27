#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <includes.h>
#include <QMainWindow>
#include "sat_track.h"
#include "TLEdownload.h"
#include "TLEParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<TLEdata> satList;
    fs::path Path;
    TLEdownload downloader;
    satellite activeSat;
    QGraphicsScene scene;
    QPixmap pixmap;
    QString s;
    int latHour, latMinute, latSecond;
    int longHour, longMinute, longSecond;
    bool latDir, longDir;
    int currentSatIndex;

private slots:
    void on_actionAbout_triggered();
    void on_actionUpdate_TLEs_triggered();
    void on_actionRefresh_TLEs_triggered();
    void on_actionQuit_triggered();
    void on_satBox_currentIndexChanged(int index);
    void on_latSecond_valueChanged(int arg1);
    void on_latMinute_valueChanged(int arg1);
    void on_latHour_valueChanged(int arg1);
    void on_longSecond_valueChanged(int arg1);
    void on_longMinute_valueChanged(int arg1);
    void on_longHour_valueChanged(int arg1);
    void on_OK_triggered(QAction *arg1);
};

#endif // MAINWINDOW_H
