#include "TLEParser.hpp"
#include "download.hpp"
#include "includes.hpp"
#include "mainwindow.hpp"
#include "sat_track.hpp"

#include <QApplication>
#include <QDebug>
#include <QtGui/QKeyEvent>

#include <locale>

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

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow a;
    a.show();

    //Ω
    return app.exec();
}
