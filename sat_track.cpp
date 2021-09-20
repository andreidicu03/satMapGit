#include <sat_track.h>

/*
         Greenwich Mean Sidereal Time (GMST) - (Old) Earth Rotation Angle

https://lweb.cfa.harvard.edu/~jzhao/times.html
"The Greenwich sidereal day begins when the vernal equinox is on the Greenwich Meridian.
Greenwich Mean Sidereal Time (GMST) is the hour angle of the average position of the vernal equinox,
neglecting short term motions of the equinox due to nutation."

Inaccuracy < 0.9s (UTC Over UT1)

More About Sidereal Time:
https://en.wikipedia.org/wiki/Sidereal_time#Sidereal_time
https://celestrak.com/columns/v02n02/
*/
satellite::satellite(){
    this->a = cbrt( (G * Me) / pow(n, 2) );
    this->t=QDateTime::currentDateTimeUtc();
    this->satDate.setTimeSpec(Qt::UTC);
    this->t.setTimeSpec(Qt::UTC);
};

satellite::satellite(std::string Epoch, float e1, float M, float n1, float w1, float W1, float i1){
    this->e=e1; // eccentricity
    this->M0=M * (M_PI/180); // mean anomaly at epoch
    this->n=n1 * (M_PI2/d2s); // mean motion (rev/day)
    this->w=w1 * (M_PI/180); // arg of perigee
    this->W=W1 * (M_PI/180); // RAAN
    this->i=i1 * (M_PI/180); // inclination
    this->a = cbrt( (G * Me) / pow(n, 2) );
    this->t=QDateTime::currentDateTimeUtc();
    this->satDate.setTimeSpec(Qt::UTC);
    this->t.setTimeSpec(Qt::UTC);
    std::string Year=Epoch.substr(0, 2), aux=Year;//
    if(stoi(Year)<57){
        Year={};
        Year="20"+aux;
        aux={};
    }
    else{
        Year={};
        Year="19"+aux;
        aux={};
    }
    std::string Day=Epoch.substr(2, 12), fracDay="0"+Day.substr(Day.find('.'), 12);//
    float hour, minute, sec, msec;
    hour=24*stof(fracDay);
    minute=60*(hour-(int)hour);
    sec=60*(minute-(int)minute);
    msec=1000*(sec-(int)sec);
    QDate date;
    QTime time;
    date.setDate(stoi(Year), 1, 1);
    date=date.addDays(stoi(Day)-1);
    time.setHMS(hour, minute, sec);
    time=time.addMSecs(msec);
    this->satDate.setDate(date);
    this->satDate.setTime(time);
}

satellite::~satellite(){
    t.~QDateTime();
    satDate.~QDateTime();
}

void satellite::satInit(std::string Epoch, float e1, float M, float n1, float w1, float W1, float i1){
    this->e=e1; // eccentricity
    this->M0=M * (M_PI/180); // mean anomaly at epoch
    this->n=n1 * (M_PI2/d2s); // mean motion (rev/day)
    this->w=w1 * (M_PI/180); // arg of perigee
    this->W=W1 * (M_PI/180); // RAAN
    this->i=i1 * (M_PI/180); // inclination
    std::string Year=Epoch.substr(0, 2), aux=Year;//
    if(stoi(Year)<57){
        Year={};
        Year="20"+aux;
        aux={};
    }
    else{
        Year={};
        Year="19"+aux;
        aux={};
    }
    std::string Day=Epoch.substr(2, 12), fracDay="0"+Day.substr(Day.find('.'), 12);//
    float hour, minute, sec, msec;
    hour=24*stof(fracDay);
    minute=60*(hour-(int)hour);
    sec=60*(minute-(int)minute);
    msec=1000*(sec-(int)sec);
    QDate date;
    QTime time;
    date.setDate(stoi(Year), 1, 1);
    date=date.addDays(stoi(Day)-1);
    time.setHMS(hour, minute, sec);
    time=time.addMSecs(msec);
    this->satDate.setDate(date);
    this->satDate.setTime(time);
}

long double satellite::rotation_angle(){
    long double jd, dn, ut, du, tu, gmst0, gmst;

    QDateTime orig; QDate origDate; QTime origTime;
    origDate.setDate(2000, 1, 1); origTime.setHMS(12, 0, 0); orig.setDate(origDate); orig.setTime(origTime); orig.setTimeSpec(Qt::UTC); //c cros cros tutoral

    jd=t.toSecsSinceEpoch() - orig.toSecsSinceEpoch();
    dn=floor((jd+0.5*d2s)/d2s);
    ut=fmod(jd+0.5*d2s,d2s);
    du = dn - 0.5;
    tu = du/36525;
    gmst0=((24110.54841 + tu * (8640184.812866 + tu * (0.093104 - tu * 6.2e-6))) / 240)*(M_PI/180);
    gmst = fmod((gmst0 + We * ut) , M_PI2);
    return gmst;
}
/*
        APPROXIMATE Eccentric Anomaly Using Newton's Method

https://en.wikipedia.org/wiki/Kepler%27s_equation#Numerical_approximation_of_inverse_problem
*/
float satellite::eccentric_anomaly(){
    this->E=0;
    float M, dt=0;
    int accuracy;
    auto equation = [](const long double E, const float e, const long double M){
      return M-E + e*sin(E);
    };
    auto derivative = [](const long double E, const float e){
      return e*cos(E)-1;
    };

    dt=t.toSecsSinceEpoch()-satDate.toSecsSinceEpoch();
    M=M0 + n * dt;
    this->E=M;
    accuracy=10;

    for(int i =0; i<accuracy; i++){
        this->E=this->E-(equation(this->E, e, M)/derivative(this->E, e));
    }
    return this->E;
}
/*
                Polar (Kepler) To PQW To ECI

From Polar To PWQ:
All of the following operations are functions of true anomaly.
https://en.wikipedia.org/wiki/Polar_coordinate_system#Converting_between_polar_and_Cartesian_coordinates

From PQW To ECI:
https://en.wikipedia.org/wiki/Orbital_elements#Euler_angle_transformations
"The inverse transformation, which computes the 3 coordinates in the X-Y-Z system,
given the 3 (or 2) coordinates in the P-Q-W system, is represented by the inverse matrix."

Visual Representation: https://www.youtube.com/watch?v=fzTJmMnTinU

Faster Methods (ECI): (Precalculate Matrices)
https://modelica.org/events/modelica2008/Proceedings/sessions/session4d1.pdf
https://downloads.rene-schwarz.com/download/M001-Keplerian_Orbit_Elements_to_Cartesian_State_Vectors.pdf
*/

coord satellite::coordinate_transformations(){
    long double f, r, X, Y, Z, P, Q;
    this->E=eccentric_anomaly();
    f=2*atan(sqrt((1+e)/(1-e))*tan(E/2));
    r=(a*(1-e*e))/(1+e*cos(f));
    P=r*cos(f);
    Q=r*sin(f);

    X=P*(cos(w)*cos(W)-sin(w)*cos(i)*sin(W))-Q*(sin(w)*cos(W)+cos(w)*cos(i)*sin(W));
    Y=P*(cos(w)*sin(W)+sin(w)*cos(i)*cos(W))+Q*(cos(w)*cos(i)*cos(W)-sin(w)*sin(W));
    Z=P*(sin(w)*sin(i))+Q*(cos(w)*sin(i));
    //std::cout<<a<<"\n";
    coord c={X, Y, Z};
    return  c;
}

latlong satellite::ground_track(QDateTime t){
    long double R, lmst;
    latlong ret;
    this->t=t;
    coord XYZ = coordinate_transformations();

    R=sqrt(XYZ.X*XYZ.X+XYZ.Y*XYZ.Y+XYZ.Z*XYZ.Z);

    ret.lat=asin(XYZ.Z/R)*(180/M_PI);

    lmst=atan(XYZ.Y/XYZ.X);
    ret.lon=(lmst-rotation_angle())*(180/M_PI);

    return ret;
}

void satellite::coutSat(){
    qDebug()<<"\n\n";
    qDebug()<<t<<satDate;
    qDebug()<<a;// semi major axis
    qDebug()<<e;// eccentricity
    qDebug()<<E;// Eccentric anomaly
    qDebug()<<M0;// mean anomaly at epoch
    qDebug()<<n;// mean motion (rev/day)
    qDebug()<<w;// arg of perigee
    qDebug()<<W;// RAAN
    qDebug()<<i;// inclination
}
