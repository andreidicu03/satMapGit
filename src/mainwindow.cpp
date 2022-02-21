#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "fileSearch.hpp"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    UtcTime.setTimeSpec(Qt::UTC);

    QTimer* updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    updateTimer->start();

    setlocale (LC_ALL,"en_US.utf8");
    ui->setupUi(this);

    latHour=0; latMinute=0; latSecond=0;
    longHour=0; longMinute=0; longSecond=0;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;

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

    QFile jsonFile("./freq.json");
    jsonFile.open(QFile::ReadOnly);
    QString raw = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(raw.toUtf8());
    satFrequencies=doc.array();

    this->readSettings();

    //QSettings settings("Barbatboss03", "satMap");

    //qDebug()<<settings.allKeys();

    //for(int i=0; i<settings.allKeys().size(); i++) qDebug()<<settings.value(settings.allKeys()[i]);

    //ui->satFreqTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->satPassesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeSettings(){
    QSettings settings("Barbatboss03", "satMap");

    settings.clear();

    settings.beginGroup("mainwindow");
    settings.setValue("homeCoordlat", QVariant::fromValue(QString::number((double)homeCoord.lat)));
    settings.setValue("homeCoordlon", QVariant::fromValue(QString::number((double)homeCoord.lon)));
    settings.setValue("homeCoordh", QVariant::fromValue(QString::number((double)homeCoord.h)));
    QString links = ui->links->toPlainText();
    QStringList linkList = links.split(QRegExp("[\n]"));
    settings.beginGroup("links");

    int TLEcount=0, MAPcount=0;

    for (QString link : linkList){
        QString key = link.section(' ', 0, 0);
        QString value = link.section(' ', 1, 1);
        settings.setValue(key, value);
        if(key.contains("TLE")) TLEcount++;
        if(key.contains("MAP")) MAPcount++;
    }

    settings.setValue("tleLinkCount", QVariant::fromValue(TLEcount));
    settings.setValue("mapLinkCount", QVariant::fromValue(MAPcount));
}

void MainWindow::readSettings(){
    QSettings settings("Barbatboss03", "satMap");

    settings.beginGroup("mainwindow");
    homeCoord.lat=settings.value("homeCoordlat").toDouble();
    homeCoord.lon=settings.value("homeCoordlon").toDouble();
    homeCoord.h=settings.value("homeCoordh").toDouble();

    longHour=(int)homeCoord.lon;
    longMinute=(int)((homeCoord.lon-(int)homeCoord.lon)*60);
    longSecond=(int)((((homeCoord.lon-(int)homeCoord.lon)*60)-(int)((homeCoord.lon-(int)homeCoord.lon)*60))*60);

    latHour=(int)homeCoord.lat;
    latMinute=(int)((homeCoord.lat-(int)homeCoord.lat)*60);
    latSecond=(int)((((homeCoord.lat-(int)homeCoord.lat)*60)-(int)((homeCoord.lat-(int)homeCoord.lat)*60))*60);

    settings.beginGroup("links");

    int TLEcount=0, MAPcount=0;

    TLEcount=settings.value("tleLinkCount").toInt();
    MAPcount=settings.value("mapLinkCount").toInt();

    for(int i=0; i<TLEcount; i++){
        QString key="TLE", value;
        key.append(QString::number(i));
        value=settings.value(key).toString();
        outLinks.append(key+" "+value);
        tleDownLinks.append(value);
    }

    for(int i=0; i<MAPcount; i++){
        QString key="MAP", value;
        key.append(QString::number(i));
        value=settings.value(key).toString();
        outLinks.append(key+" "+value);
        mapDownLinks.append(value);
    }

    QString text=outLinks.join("\n");

    QTimer wait1;

    downloader1.setPath(QString::fromStdString(tlePath.string()));
    downloader1.setURLs(tleDownLinks);
    wait1.singleShot(0, &downloader1, SLOT(execute()));

    QTimer wait2;

    downloader2.setPath(QString::fromStdString(mapPath.string()));
    downloader2.setURLs(mapDownLinks);
    wait2.singleShot(0, &downloader2, SLOT(execute()));

    ui->latHour->setValue(latHour);
    ui->latMinute->setValue(latMinute);
    ui->latSecond->setValue(latSecond);

    ui->longHour->setValue(longHour);
    ui->longMinute->setValue(longMinute);
    ui->longSecond->setValue(longSecond);

    ui->heightSpinBox->setValue(homeCoord.h);

    ui->links->setText(text);

}

void MainWindow::updateTime()
{    
    UtcTime=QDateTime::currentDateTimeUtc();
    activeSat.updateTime(UtcTime);

    latlong LLH=activeSat.LLH();
    latlong ENU=activeSat.ENU();
    //qDebug()<<(double)ENU.lat<<" "<<(double)ENU.lon;
    QString llhOUT="lat: " + QString::number(LLH.lat, 'g', 4) + "° lon: " + QString::number(LLH.lon, 'g', 4)
            + "° h:" + QString::number(LLH.h, 'g', 4) + "km";

    ui->coordLabel->setText(llhOUT);

    QString enuOUT="az: " + QString::number(ENU.lon, 'g', 4) + "° el: " + QString::number(ENU.lat, 'g', 4)+ "°";

    ui->azElLabel->setText(enuOUT);

    QThread::msleep(50);
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
            newSat.satInit(i.SatName, i.CatNo, i.Epoch, i.Eccentricity, i.MeanAnomaly, i.MeanMotion, i.ArgPerigee, i.RAAN, i.Inclination, this->UtcTime, homeCoord);
            break;
        }
    }
    activeSat=newSat;
    ui->satFreqTable->setRowCount(0);
    int k=0;
    for(int i =0; i<satFrequencies.size(); i++){

        QJsonValue val=satFrequencies.at(i);
        if(val.toObject().value("norad_cat_id").toInt()==activeSat.catalogNr){
            ui->satFreqTable->insertRow(k);

            QString Description = val.toObject().value("description").toString();
            QString Mode = val.toObject().value("mode").toString();
            double Downlink = val.toObject().value("downlink_low").toDouble()/1000000;
            double Uplink = val.toObject().value("uplink_low").toDouble()/1000000;
            bool Inverted = val.toObject().value("invert").toBool();

            QTableWidgetItem *cell;

            cell = new QTableWidgetItem;
            cell->setText(Description);
            ui->satFreqTable->setItem(k, 0, cell);

            cell = new QTableWidgetItem;
            cell->setText(Mode);
            ui->satFreqTable->setItem(k, 1, cell);

            cell = new QTableWidgetItem;
            cell->setText(QString::number(Downlink));
            ui->satFreqTable->setItem(k, 2, cell);

            cell = new QTableWidgetItem;
            cell->setText(QString::number(Uplink));
            ui->satFreqTable->setItem(k, 3, cell);

            cell = new QTableWidgetItem;
            cell->setText(QString::number(Inverted));
            ui->satFreqTable->setItem(k, 4, cell);

            k++;
        }
    }
    ui->satFreqTable->resizeColumnsToContents();

    QStringList satPasses=activeSat.passPredict(24, 10);

    ui->satPassesTable->setRowCount(0);

    for(int i =0; i<satPasses.size(); i++){
        ui->satPassesTable->insertRow(i);

        QString currentPass=satPasses[i];
        QStringList properties= currentPass.split(";", QString::SkipEmptyParts);

        QTableWidgetItem *cell;

        cell = new QTableWidgetItem;
        cell->setText(properties[0]); //id
        ui->satPassesTable->setItem(i, 0, cell);

        cell = new QTableWidgetItem;
        cell->setText(properties[1]); //startTime
        ui->satPassesTable->setItem(i, 1, cell);

        cell = new QTableWidgetItem;
        cell->setText(properties[2]); //startAz
        ui->satPassesTable->setItem(i, 2, cell);

        cell = new QTableWidgetItem;
        cell->setText(properties[3]); //maxEl
        ui->satPassesTable->setItem(i, 3, cell);

        cell = new QTableWidgetItem;
        cell->setText(properties[4]); //endAz
        ui->satPassesTable->setItem(i, 4, cell);

        cell = new QTableWidgetItem;
        cell->setText(properties[5]); //endTime
        ui->satPassesTable->setItem(i, 5, cell);

        cell = new QTableWidgetItem;
        cell->setText(properties[6]); //duration
        ui->satPassesTable->setItem(i, 6, cell);
    }

    ui->satPassesTable->resizeColumnsToContents();
    //homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    //homeCoord.lon=longHour+longMinute/60+longSecond/3600;

    //activeSat.coutSat();
    //qDebug()<<(double)activeSat.ENU().lat<<" "<<(double)activeSat.ENU().lon;
}

void MainWindow::on_latSecond_valueChanged(int arg1)
{
    latSecond=arg1;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;
}

void MainWindow::on_latMinute_valueChanged(int arg1)
{
    latMinute=arg1;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;
}

void MainWindow::on_latHour_valueChanged(int arg1)
{
    latHour=arg1;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;
}

void MainWindow::on_longSecond_valueChanged(int arg1)
{
    longSecond=arg1;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;
}

void MainWindow::on_longMinute_valueChanged(int arg1)
{
    longMinute=arg1;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;
}

void MainWindow::on_longHour_valueChanged(int arg1)
{
    longHour=arg1;
    homeCoord.lat=latHour+latMinute/60+latSecond/3600;
    homeCoord.lon=longHour+longMinute/60+longSecond/3600;
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
    this->writeSettings();
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


void MainWindow::on_actionUpdate_Freq_triggered()
{

    bool ok=true;
    QString text = "https://db.satnogs.org/api/transmitters/?format=json";
    if(ok){

        QStringList URLs;
        URLs.append(text);
        downloader.setPath("./");
        downloader.setURLs(URLs);
        downloader.saveFileName("./freq.json");
        QTimer wait;
        wait.singleShot(0, &downloader, SLOT(execute()));
    }


}


void MainWindow::on_longChoose_currentIndexChanged(int index)
{
    if(index){
        homeCoord.lon=(-1)*(longHour+longMinute/60+longSecond/3600);
    }
    else
        homeCoord.lon=(longHour+longMinute/60+longSecond/3600);
}


void MainWindow::on_latChoose_currentIndexChanged(int index)
{
    if(index){
        homeCoord.lat=(-1)*(latHour+latMinute/60+latSecond/3600);
    }
    else
        homeCoord.lat=(latHour+latMinute/60+latSecond/3600);
}

