#ifndef SAT_TRACK_H
#define SAT_TRACK_H

#include "includes.h"



class satellite {
private:
    QString Name;
    int catalogNr;
    QDateTime t, satDate;
    float a;// semi major axis
    float e;// eccentricity
    float E;// Eccentric anomaly
    float M0;// mean anomaly at epoch
    float n;// mean motion (rev/day)
    float w;// arg of perigee
    float W;// RAAN
    float i;// inclination
public:
    satellite();
    ~satellite();
    satellite(std::string Name, int catNr, std::string Epoch, float e, float M0, float n, float w, float W, float i);
    void satInit(std::string Name, int catNr, std::string Epoch, float e1, float M, float n1, float w1, float W1, float i1);
    long double rotation_angle();
    float eccentric_anomaly();
    coord coordinate_transformations();
    latlong ground_track(QDateTime t);
    void coutSat();
};

#endif // SAT_TRACK_H
