#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Path="./";
    ui->satLabel->setText("");
    satList=SatSearch(Path);
    for(TLEdata &i: satList){
        QString satN=QString::fromStdString(i.SatName);
        ui->satBox->addItem(satN, i.index);
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
    if(index==ui->satBox->itemData(index)){
        qDebug()<<"YO\n";
    }
}

