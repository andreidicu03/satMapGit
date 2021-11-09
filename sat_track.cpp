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

satellite::satellite(std::string Name, int catNr, std::string Epoch, float e1, float M, float n1, float w1, float W1, float i1){
    this->catalogNr=catNr;
    this->Name=QString::fromStdString(Name);
    this->e=e1; // eccentricity
    this->M0=M * (M_PI/180); // mean anomaly at epoch
    this->n=n1 * (M_PI2/d2s); // mean motion (rev/day)
    this->ω=w1 * (M_PI/180); // arg of perigee
    this->Ω=W1 * (M_PI/180); // RAAN
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

void satellite::satInit(std::string Name, int catNr, std::string Epoch, float e1, float M, float n1, float w1, float W1, float i1){

    this->catalogNr=catNr;
    this->Name=QString::fromStdString(Name);
    this->e=e1; // eccentricity
    this->M0=M * (M_PI/180); // mean anomaly at epoch
    this->n=n1 * (M_PI2/d2s); // mean motion (rev/day)
    this->ω=w1 * (M_PI/180); // arg of perigee
    this->Ω=W1 * (M_PI/180); // RAAN
    this->i=i1 * (M_PI/180); // inclination
    this->a = cbrt( (G * Me) / pow(n, 2) );
    this->t=QDateTime::currentDateTimeUtc();
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
    this->satDate.setTimeSpec(Qt::UTC);
    std::string Day=Epoch.substr(2, 12), fracDay="0"+Day.substr(Day.find('.'), 12);//
    float hour=0, minute=0, sec=0, msec=0;
    hour=24*std::stof(fracDay);
    minute=60*(hour-(int)hour);
    sec=60*(minute-(int)minute);
    msec=1000*(sec-(int)sec);
    QDate date;
    QTime time;
    date.setDate(stoi(Year), 1, 1);
    date=date.addDays(stoi(Day)-1);
    time.setHMS(hour, minute, sec);
    time=time.addMSecs(msec);
    qDebug()<<std::atof("0.9999");
    qDebug()<<QString::fromStdString(fracDay);
    qDebug()<<hour<<minute<<sec<<msec;
    qDebug()<<time;
    this->satDate.setDate(date);
    this->satDate.setTime(time);
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
    qDebug()<<this->E;
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

QGenericMatrix<1,3,float> satellite::ECI(){
    // calculate the semi-major axis from kepler's 3rd law
    //b = a * np.sqrt( 1 - np.power(e, 2) ) # semi-minor axis

    // true anomaly [rad] & radius [m]
    float ν = 2 * atan(sqrt((1 + e) / (1 - e)) * tan(eccentric_anomaly() / 2));
    float r = (a * (1 - pow(e, 2))) / (1 + e * cos(ν));

    QGenericMatrix<1,3,float> PQW, ECI;

    float val []={(float)cos(ν), (float)sin(ν), 0};

    QGenericMatrix<1,3,float> c(val);

    PQW = r * c;

    ECI = (RotateZ(-Ω) * RotateX(-i) * RotateZ(-ω)) * PQW;
    //qDebug()<<ν;
    return PQW;
}

void satellite::coutSat(){
    qDebug()<<"\n\n";
    qDebug()<<Name;
    qDebug()<<catalogNr;
    qDebug()<<t<<satDate;
    qDebug()<<a;// semi major axis
    qDebug()<<e;// eccentricity
    qDebug()<<E;// Eccentric anomaly
    qDebug()<<M0;// mean anomaly at epoch
    qDebug()<<n;// mean motion (rev/day)
    qDebug()<<ω;// arg of perigee
    qDebug()<<Ω;// RAAN
    qDebug()<<i;// inclination
}
