#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileSearch.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    latHour=0; latMinute=0; latSecond=0;
    longHour=0; longMinute=0; longSecond=0;

    tlePath="./tle";
    mapPath="./map";

    if(!fs::exists(tlePath)){
        fs::create_directory(tlePath);
    }

    if(!fs::exists(mapPath)){
        fs::create_directory(mapPath);
    }

    activeTLE="";

    tleFiles=fileSearch(QString::fromStdString(tlePath.string()), ".txt");
    for(QString &i : tleFiles)
    {
        ui->tleSelect->addItem(i);
    }
    ui->satBox->clear();

    ui->latChoose->addItem("N", 0);
    ui->latChoose->addItem("S", 1);
    ui->longChoose->addItem("E", 0);
    ui->longChoose->addItem("W", 1);

    mapFiles=fileSearch(QString::fromStdString(mapPath.string()), ".png");
    ui->mapSelect->clear();
    for(QString &i : mapFiles)
    {
        ui->mapSelect->addItem(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox about;
    about.setText("This program is made for tracking satellites for amateur radio use\n"
                  "\n"
                  "\n"
                  "\n"
                  "\n"
                  "Made by Barbatboss03 and Sebalo13");
    about.exec();
}

void MainWindow::on_actionUpdate_TLEs_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("URL:"), QLineEdit::Normal,
                                         "https://celestrak.com/NORAD/elements/active.txt", &ok);
    if(ok){
        QStringList URLs;
        URLs.append(text);
        downloader.setURLs(URLs);
        downloader.setPath(QString::fromStdString(tlePath.string()));
        QTimer wait;
        wait.singleShot(0, &downloader, SLOT(execute()));

    }
    else{
        QMessageBox* error = new QMessageBox();
        error->setText("Download Error");
        error->exec();
    }
    ui->tleSelect->clear();
    tleFiles=fileSearch(QString::fromStdString(tlePath.string()), ".txt");
    for(QString &i : tleFiles)
    {
        ui->tleSelect->addItem(i);
    }
}


void MainWindow::on_actionRefresh_TLEs_triggered()
{
    ui->tleSelect->clear();
    tleFiles=fileSearch(QString::fromStdString(tlePath.string()), ".txt");
    for(QString &i : tleFiles)
    {
        ui->tleSelect->addItem(i);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}


void MainWindow::on_satBox_currentIndexChanged(int index)
{
    satellite newSat;
    for(TLEdata &i: satList){
        if(index==i.index){
            newSat.satInit(i.SatName, i.CatNo, i.Epoch, i.Eccentricity, i.MeanAnomaly, i.MeanMotion, i.ArgPerigee, i.RAAN, i.Inclination);
            break;
        }
    }
    activeSat=newSat;
    //activeSat.coutSat();
}

void MainWindow::on_latSecond_valueChanged(int arg1)
{
    latSecond=arg1;
}

void MainWindow::on_latMinute_valueChanged(int arg1)
{
    latMinute=arg1;
}

void MainWindow::on_latHour_valueChanged(int arg1)
{
    latHour=arg1;
}

void MainWindow::on_longSecond_valueChanged(int arg1)
{
    longSecond=arg1;
}

void MainWindow::on_longMinute_valueChanged(int arg1)
{
    longMinute=arg1;
}

void MainWindow::on_longHour_valueChanged(int arg1)
{
    longHour=arg1;
}

void MainWindow::on_actionDownload_map_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("URL:"), QLineEdit::Normal,
                                         "https://upload.wikimedia.org/wikipedia/commons/thumb/a/a7/Large_World_Physical_Map.png/2560px-Large_World_Physical_Map.png", &ok);
    if(ok){

        QStringList URLs;
        URLs.append(text);
        downloader.setPath(QString::fromStdString(mapPath.string()));
        downloader.setURLs(URLs);
        QTimer wait;
        wait.singleShot(0, &downloader, SLOT(execute()));

    }
    else{
        QMessageBox* error = new QMessageBox();
        error->setText("Download Error");
        error->exec();
    }
    mapFiles.clear();
    mapFiles=fileSearch(QString::fromStdString(mapPath.string()), ".png");
    ui->mapSelect->clear();
    for(QString &i : mapFiles)
    {
        ui->mapSelect->addItem(i);
    }
}

void MainWindow::on_mapOK_clicked()
{
    pixmap.load(ui->mapSelect->currentText());
    scene.clear();
    scene.addPixmap(pixmap);
    ui->mapView->setScene(&scene);
    ui->mapView->show();
}


void MainWindow::on_OK_clicked()
{

}


void MainWindow::on_tleOK_clicked()
{
    activeTLE=ui->tleSelect->currentText().toStdString();
    ui->satBox->clear();
    satList=SatSearch(activeTLE);
    for(TLEdata &i: satList){
        QString satN=QString::fromStdString(i.SatName);
        ui->satBox->addItem(satN, i.index);
    }
}


void MainWindow::on_actionRefresh_maps_triggered()
{
    mapFiles=fileSearch(QString::fromStdString(mapPath.string()), ".png");
    ui->mapSelect->clear();
    for(QString &i : mapFiles)
    {
        ui->mapSelect->addItem(i);
    }
}

