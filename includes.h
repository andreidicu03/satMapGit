#ifndef INCLUDES_H
#define INCLUDES_H

#include <QObject>
#include <QtCore>
#include <QString>
#include <QGeoRoute>

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <filesystem>

#define M_PI2 M_PI * 2

inline const int d2s = 86400;
inline const double Ee = 6378.137e3; // equatorial radius [m]
inline const double Pe = 6356.752e3; // polar radius [m]
inline const double Me = 5.97240e24; // mass [kg]
inline const double We = 7.2921e-05; // rotation rate [rad/s]
inline const double G = 6.67430e-11; // [m^3 kg^-1 s^-2]

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
};

struct coord{
    long double X, Y, Z;
};

struct latlong{
    long double lat, lon;
};

#endif // INCLUDES_H
