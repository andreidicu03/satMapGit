#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    latHour=0; latMinute=0; latSecond=0;
    longHour=0; longMinute=0; longSecond=0;
    Path="./";
    ui->satLabel->setText("");
    satList=SatSearch(Path);
    ui->latChoose->addItem("N", 0);
    ui->latChoose->addItem("S", 1);
    ui->longChoose->addItem("E", 0);
    ui->longChoose->addItem("W", 1);
    for(TLEdata &i: satList){
        QString satN=QString::fromStdString(i.SatName);
        ui->satBox->addItem(satN, i.index);
    }
    pixmap.load("./2560x1280map.png");
    scene.addPixmap(pixmap);
    ui->mapView->setScene(&scene);
    ui->mapView->show();
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
        QTimer wait;
        wait.singleShot(0, &downloader, SLOT(execute()));

    }
    else{
        QMessageBox* error = new QMessageBox();
        error->setText("Download Error");
        error->exec();
    }
}


void MainWindow::on_actionRefresh_TLEs_triggered()
{
    satList=SatSearch(Path);
    for(TLEdata &i: satList){
        QString satN=QString::fromStdString(i.SatName);
        ui->satBox->addItem(satN);
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
            newSat.satInit(i.Epoch, i.Eccentricity, i.MeanAnomaly, i.MeanMotion, i.ArgPerigee, i.RAAN, i.Inclination);
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

void MainWindow::on_OK_triggered(QAction *arg1)
{

}

