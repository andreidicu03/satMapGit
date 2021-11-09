#include <marble/MarbleWidget.h>
#include <marble/MarbleMap.h>
#include <marble/MarbleModel.h>
#include <marble/GeoPainter.h>
#include <marble/LayerInterface.h>
#include <marble/GeoDataLineString.h>

#include "mainwindow.h"
#include "sat_track.h"
#include "download.h"
#include "TLEParser.h"

#include <QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>

#include <locale>

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
//pt glob se aplica ca la harta

float JD(QDateTime t){
    QDateTime orig; QDate origDate; QTime origTime;
    origDate.setDate(2000, 1, 1); origTime.setHMS(12, 0, 0); orig.setDate(origDate); orig.setTime(origTime); orig.setTimeSpec(Qt::UTC); //c cros cros tutoral

    return t.toSecsSinceEpoch() - orig.toSecsSinceEpoch();
}

float TT(QDateTime t){
    float TT = (JD(t)/86400) / 36525;
    return TT;
}


int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    QList<TLEdata> satList;
    fs::path Path("./");
    satList=SatSearch(Path);
    //latlong lmao;

    std::cout.precision(30);

    MainWindow a;
    a.show();

    //Ω
    return app.exec();
}
