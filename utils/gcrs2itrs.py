import numpy as np
from nutation_table import C
from constants import We
from rotation_matrices import RX, RY, RZ


"""

ECI (GCRS) TO ECR (ITRS)

https://gssc.esa.int/navipedia/index.php/Transformation_between_Celestial_and_Terrestrial_Frames
https://www.iers.org/SharedDocs/Publikationen/EN/IERS/Publications/tn/TechnNote21/tn21_20.pdf?__blob=publicationFile&v=1

Q(T) - Nutation (IAU 1980) & Precession (IAU 1976) Models
https://en.wikipedia.org/wiki/Ecliptic
R(T) - Earth's Rotation (GAST)
W(T) - Polar Motion (From IERS)
https://hpiers.obspm.fr/eoppc/eop/eopc01/filtered-pole.tab
"""

def N(T):

    T2, T3 = np.power(T, (2, 3))

    a = [ # fundamental arguments [as]
    # moon's mean anomaly - l
    485866.733 + (1.7172e9 + 715922.633) * T + 31.310 * T2 + 0.064 * T3,
    # sun's mean anomaly - l'
    1287099.804 + (1.28304e8 + 1292581.224) * T - 0.577 * T2 - 0.012 * T3,
    # moon's mean argument of latitude - F
    335778.877 + (1.739232e9 + 295263.137) * T - 13.257 * T2 + 0.011 * T3,
    # moon's mean elongation from the sun - D
    1072261.307 + (1.601856e9 + 1105601.328) * T - 6.891 * T2 + 0.019 * T3,
    # mean longitude of the ascending lunar node - Ω
    450160.280 - (6.48e6 + 482890.539) * T + 7.455 * T2 + 0.008 * T3
        ]

    # mean obliquity [as]
    ε = 84381.448 - 46.8150 * T - 0.00059 * T2 + 0.001813 * T3

    # nutation angles in longitude and obliquity [as]
    Δψ, Δε = 0, 0 # prediction using the nutation table (C)
    for j in range(len(C)):

        # from [as] to [rad]
        ARG = np.radians(np.divide(np.sum([C[j][i] * a[i] for i in range(5)]), 3600))

        # from [mas] to [as]
        Δψ += (C[j][5] + C[j][6] * T) * np.sin(ARG) * 1e-3
        Δε += (C[j][7] + C[j][8] * T) * np.cos(ARG) * 1e-3

    # from [as] to [rad]
    ε, Δψ, Δε = np.radians(np.divide((ε, Δψ, Δε), 3600))

    return ε, Δψ, Δε

def P(T):

    T2, T3 = np.power(T, (2, 3))

    z = 2306.2181 * T + 1.09468 * T2 + 0.018203 * T3
    ϑ = 2004.3109 * T - 0.42665 * T2 - 0.041833 * T3
    ζ = 2306.2181 * T + 0.30188 * T2 + 0.017998 * T3

    # from [as] to [rad]
    z, ϑ, ζ = np.radians(np.divide((z, ϑ, ζ), 3600))

    return z, ϑ, ζ

def Q(T):

    ε, Δψ, Δε = N(T)
    N_M = RX(-(ε + Δε)) @ RZ(-Δψ) @ RX(ε)

    z, ϑ, ζ = P(T)
    P_M = RZ(-z) @ RY(ϑ) @ RZ(-ζ)

    Q = N_M @ P_M

    return Q

def W(T):

    # angular displacements [as]
    X, Y = np.radians(np.divide((0.125751, -0.358457), 3600))

    W = RY(-X) @ RX(-Y)

    return W

"""
            Greenwich Mean Sidereal Time (GMST) - (Old) Earth Rotation Angle

https://lweb.cfa.harvard.edu/~jzhao/times.html
"The Greenwich sidereal day begins when the vernal equinox is on the Greenwich Meridian.
Greenwich Mean Sidereal Time (GMST) is the hour angle of the average position of the vernal equinox,
neglecting short term motions of the equinox due to nutation."

More About Sidereal Time:
https://en.wikipedia.org/wiki/Sidereal_time#Sidereal_time
https://celestrak.com/columns/v02n02/
"""

def H(J):

    # dn - days (number) difference [days], ut - (universal) time [s]
    D0, UT1 = np.divmod(J - 0.5 * 86400, 86400) # - 43200: offset for counting days from 0:00 (midnight)
    Du = D0 + 0.5 # correct for measurement from 12:00 (noon)

    # tu - julian centuries from du [days]
    Tu = Du / 36525

    # H0 (gmst0) - greenwich mean sidereal time at midnight [rad]
    H0 = np.radians((24110.54841 + Tu * (8640184.812866 + Tu * (0.093104 - Tu * 6.2e-6))) / 240) # / 240: from [s] to [deg]
    H = H0 + We * UT1

    return H

def ΔH(T):

    # equation of equinoxes
    ε, Δψ, Δε = N(T)
    ΔH = Δψ * np.cos(ε) # ? = np.arctan(np.tan(Δψ) * np.cos(ε + Δε))

    return ΔH