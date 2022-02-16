#ifndef INCLUDES_H
#define INCLUDES_H

#include <QObject>
#include <QtCore>
#include <QString>
#include <QGeoRoute>
#include <QGraphicsScene>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QSettings>
#include <QThread>

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <filesystem>

#define M_PI2 M_PI * 2

namespace fs=std::filesystem;

inline const int d2s = 86400;
inline const double Ee = 6378.137e3; // equatorial radius [m]
inline const double Pe = 6356.752e3; // polar radius [m]
inline const double Me = 5.97240e24; // mass [kg]
inline const double We = 7.2921e-05; // rotation rate [rad/s]
inline const double G = 6.67430e-11; // [m^3 kg^-1 s^-2]

inline const double f = 1 - (Pe/Ee);
inline const double e2 = f * (2 - f); //earth's first eccentricity squared = 6.69437999014e-3

inline const double TAI_UTC = 37; // [s]
inline const double TT_TAI = 32.184;// [s]

struct TLEdata{
    //title line
    std::string SatName;
    //line1
    std::string IntDesignator; //launch date and launch piece
    std::string Epoch;
    //line2
    int CatNo;
    float Inclination;
    float RAAN;//Right Ascension of the Ascending Node
    float Eccentricity;
    float ArgPerigee;
    float MeanAnomaly;
    double MeanMotion;
    //file
    std::string origFile;
    int index;
};

struct coord{
    long double X, Y, Z;
};

struct latlong{
    long double lat, lon, h; //when in use in function ENU() lon=Azimuth, lat=Elevation
};

inline latlong homeCoord;
inline float height;
/*template <typename T>

T** MatrixMult (T** m1, T** m2){

}
*/
#endif // INCLUDES_H
