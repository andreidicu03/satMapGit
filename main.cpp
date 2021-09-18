#include <marble/MarbleWidget.h>
#include <marble/MarbleMap.h>
#include <marble/MarbleModel.h>
#include <marble/GeoPainter.h>
#include <marble/LayerInterface.h>
#include <marble/GeoDataLineString.h>

#include "mainwindow.h"
#include "sat_track.h"
#include "TLEdownload.h"
#include "TLEParser.h"

#include <QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>

#include <includes.h>

using namespace Marble;

//https://upload.wikimedia.org/wikipedia/commons/a/a7/Large_World_Physical_Map.png
//https://upload.wikimedia.org/wikipedia/commons/thumb/a/a7/Large_World_Physical_Map.png/2560px-Large_World_Physical_Map.png

/*class MyMarbleWidget : public MarbleWidget
{
public:
    virtual void customPaint(GeoPainter* painter){
        GeoDataCoordinates France( 2.2, 48.52, 0.0, GeoDataCoordinates::Degree );
            painter->setPen( QColor( 0, 0, 0 ) );
            painter->drawText( France, "France" );

            GeoDataCoordinates Canada( -77.02, 48.52, 0.0, GeoDataCoordinates::Degree );
            painter->setPen( QColor( 0, 0, 0 ) );
            painter->drawText( Canada, "Canada" );

            //A line from France to Canada without tessellation

            GeoDataLineString shapeNoTessellation( NoTessellation );
            shapeNoTessellation << France << France;

            painter->setPen( Qt::blue );
            painter->drawPolyline( shapeNoTessellation );

            //The same line, but with tessellation

            GeoDataLineString shapeTessellate( Tessellate );
            shapeTessellate << France << Canada;

            painter->setPen( Qt::red );
            painter->drawPolyline( shapeTessellate );

            //Now following the latitude circles

            GeoDataLineString shapeLatitudeCircle( RespectLatitudeCircle | Tessellate );
            shapeLatitudeCircle << France << Canada;
            painter->setPen( Qt::green );
            painter->drawPolyline( shapeLatitudeCircle );
    }
};*/

//pt harta si orbita desenez pe harta toate pozitiile satelitului cu jumatate de zi in trecut si jumatate de zi in viitor de la timpul curent
//pt tabel ????
//pt frecvente ????
//pt glob se aplica ca la harta

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    TLEdownload downloader;
    //std::cout<<Me<<"\n";
    QTimer::singleShot(0, &downloader, SLOT(execute()));
    //double PS=6607592.466474319, e2=0.00669437999014, Z=-1609934.5401485646, Ee=6378137.0;
    //std::cout<<sqrt(pow(pow(PS, 2) + (1 - e2) * pow(Z, 2) * pow(10, 2), 3)) / (Ee * e2)<<"\n";
    std::vector<TLEdata> satList;
    fs::path testPath("/home/berceni/build-satMap-Desktop-Debug/");
    satList=SatSearch(testPath);
    latlong lmao;

    std::cout.precision(30);

    for(TLEdata& i : satList){
        if(i.CatNo==25544&&i.origFile=="active.txt") {
            satellite ISS(i.Epoch, i.Eccentricity, i.MeanAnomaly, i.MeanMotion, i.ArgPerigee, i.RAAN, i.Inclination);
            lmao=ISS.ground_track(QDateTime::currentDateTimeUtc());
            std::cout<<lmao.lat<<" "<<lmao.lon<<"\n";
        }
    }

    //MyMarbleWidget *mapWidget = new MyMarbleWidget;
    MainWindow table;

    //mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    //mapWidget->show();

    table.show();

    //Ω
    return app.exec();
}
