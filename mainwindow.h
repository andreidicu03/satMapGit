#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "includes.h"
#include "sat_track.h"
#include "download.h"
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

    QDateTime UtcTime;

    QGraphicsScene scene;
    QPixmap pixmap;

    QList<TLEdata> satList;
    satellite activeSat;

    QStringList mapFiles, tleFiles;
    fs::path mapPath, tlePath, activeTLE, conf;
    download downloader;

    QJsonArray satFrequencies;

    latlong homeCoord;
    int latHour, latMinute, latSecond;
    int longHour, longMinute, longSecond;
    bool latDir, longDir;

    int currentSatIndex;

private slots:
    void updateTime();
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
    void on_actionDownload_map_triggered();
    void on_mapOK_clicked();
    void on_OK_clicked();
    void on_tleOK_clicked();
    void on_actionRefresh_maps_triggered();
    void on_actionUpdate_Freq_triggered();
};

#endif // MAINWINDOW_H
