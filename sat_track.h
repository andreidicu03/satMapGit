#ifndef SAT_TRACK_H
#define SAT_TRACK_H

#include "includes.h"
#include "gcrs2itrs.cpp"

class satellite {
private:
    QString Name;
    QDateTime t, satDate;
    float a;//semi-major axis
    float e;// eccentricity
    float E;// Eccentric anomaly
    float M0;// mean anomaly at epoch
    float n;// mean motion (rev/day)
    float ω;// arg of perigee
    float Ω;// RAAN
    float i;// inclination
public:
    int catalogNr;
    satellite();
    ~satellite();
    satellite(std::string Name, int catNr, std::string Epoch, float e, float M0, float n, float w, float W, float i, QDateTime devTime);
    void satInit(std::string Name, int catNr, std::string Epoch, float e1, float M, float n1, float w1, float W1, float i1, QDateTime devTime);
    float eccentric_anomaly();
    QGenericMatrix<1,3,float> ECR();
    coord coordinate_transformations();
    latlong ground_track();
    void coutSat();
    void updateTime(QDateTime devTime);
};

#endif // SAT_TRACK_H
