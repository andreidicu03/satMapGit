#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "includes.hpp"
#include "sat_track.hpp"
#include "download.hpp"
#include "TLEParser.hpp"

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
    fs::path mapPath, tlePath, activeTLE;
    download downloader, downloader1, downloader2;

    QJsonArray satFrequencies;

    latlong homeCoord;
    float latHour, latMinute, latSecond;
    float longHour, longMinute, longSecond;
    bool latDir, longDir;
    QStringList outLinks, tleDownLinks, mapDownLinks;

    int currentSatIndex;

private slots:
    void writeSettings();
    void readSettings();
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
    void on_longChoose_currentIndexChanged(int index);
    void on_latChoose_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
