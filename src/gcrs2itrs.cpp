#ifndef GCRS2ITRS_CPP
#define GCRS2ITRS_CPP

#include "gcrs2itrs.hpp"

inline void N(float T, float &ε, float &Δψ, float &Δε)
{
    float T2 = T * T;
    float T3 = T2 * T;
    float ARG;

    double a[5] = {// fundamental arguments [as]
                   // moon's mean anomaly - l
                   485866.733 + (1.7172e9 + 715922.633) * T + 31.310 * T2 + 0.064 * T3,
                   // sun's mean anomaly - l'
                   1287099.804 + (1.28304e8 + 1292581.224) * T - 0.577 * T2 - 0.012 * T3,
                   // moon's mean argument of latitude - F
                   335778.877 + (1.739232e9 + 295263.137) * T - 13.257 * T2 + 0.011 * T3,
                   // moon's mean elongation from the sun - D
                   1072261.307 + (1.601856e9 + 1105601.328) * T - 6.891 * T2 + 0.019 * T3,
                   // mean longitude of the ascending lunar node - Ω
                   450160.280 - (6.48e6 + 482890.539) * T + 7.455 * T2 + 0.008 * T3};

    // mean obliquity [as]
    ε = 84381.448 - 46.8150 * T - 0.00059 * T2 + 0.001813 * T3;

    // nutation angles in longitude and obliquity [as]
    Δψ = 0;
    Δε = 0;
    // prediction using the nutation table (C)
    for (int j = 0; j < 106; j++) {
        ARG = 0;
        // from [as] to [rad]
        for (int i = 0; i < 5; i++)
            ARG += N_Table[j][i] * a[i];

        ARG = (ARG / 3600) * (M_PI / 180);

        // from [mas] to [as]
        Δψ += (N_Table[j][5] + N_Table[j][6] * T) * sin(ARG) * 1e-3;
        Δε += (N_Table[j][7] + N_Table[j][8] * T) * cos(ARG) * 1e-3;

        // from [as] to [rad]
    }
    ε = (ε / 3600) * (M_PI / 180);
    Δψ = (Δψ / 3600) * (M_PI / 180);
    Δε = (Δε / 3600) * (M_PI / 180);
}

inline void P(float T, float &z, float &ϑ, float &ζ)
{
    float T2 = T * T;
    float T3 = T2 * T;

    z = 2306.2181 * T + 1.09468 * T2 + 0.018203 * T3;
    ϑ = 2004.3109 * T - 0.42665 * T2 - 0.041833 * T3;
    ζ = 2306.2181 * T + 0.30188 * T2 + 0.017998 * T3;

    // from [as] to [rad]
    z = z / 3600 * (M_PI / 180);
    ϑ = ϑ / 3600 * (M_PI / 180);
    ζ = ζ / 3600 * (M_PI / 180);
}

inline QMatrix3x3 Q(float T)
{
    float ε;
    float Δψ;
    float Δε;
    N(T, ε, Δψ, Δε);
    QMatrix3x3 N_M = RotateX(-(ε + Δε)) * RotateZ(-Δψ) * RotateX(ε);

    float z;
    float ϑ;
    float ζ;
    P(T, z, ϑ, ζ);
    QMatrix3x3 P_M = RotateZ(-z) * RotateY(ϑ) * RotateZ(-ζ);

    QMatrix3x3 Q = N_M * P_M;

    return Q;
}

inline QMatrix3x3 W()
{
    float X = 0.125751 / 3600 * (M_PI / 180);
    float Y = -0.358457 / 3600 * (M_PI / 180);

    QMatrix3x3 W = RotateY(-X) * RotateX(-Y);

    return W;
}

inline float H(float JDt)
{
    float H;
    int D0, UT1;
    std::div_t dv = std::div(JDt - 0.5 * 86400, 86400);
    D0 = dv.quot;
    UT1 = dv.rem;

    float Du;
    Du = D0 + 0.5;

    float Tu;
    Tu = Du / 36525;

    float H0;
    H0 = ((24110.54841 + Tu * (8640184.812866 + Tu * (0.093104 - Tu * 6.2e-6))) / 240)
         * (M_PI / 180);
    H = H0 + We * UT1;

    return H;
}

inline float ΔH(float T)
{
    float ΔH;
    float ε, Δψ, Δε;
    N(T, ε, Δψ, Δε);
    ΔH = Δψ * cos(ε);

    return ΔH;
}

#endif
