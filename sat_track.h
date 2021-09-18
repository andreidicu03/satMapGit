#ifndef SAT_TRACK_H
#define SAT_TRACK_H

#include "includes.h"



class satellite {
private:
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
    satellite(std::string Epoch, float e, float M0, float n, float w, float W, float i);
    long double rotation_angle();
    float eccentric_anomaly();
    coord coordinate_transformations();
    latlong ground_track(QDateTime t);

};

#endif // SAT_TRACK_H
