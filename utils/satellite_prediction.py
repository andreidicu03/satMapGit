import celestrak_request
from nutation_table import *
from rotation_matrices import RX, RY, RZ
from constants import *
import numpy as np
from datetime import datetime
from gcrs2itrs import W, H, ΔH, Q


s, _, t0, n, e, i, Ω, ω, M0, *_ = celestrak_request.get_satellite(25544)

n *= PI2 / D2S # convert from [rev/day] to [rad/s]
i, Ω, ω, M0 = np.radians( (i, Ω, ω, M0) ) # convert from [deg] to [rad]


"""
APPROXIMATE Eccentric Anomaly Using Newton's Method

https://en.wikipedia.org/wiki/Kepler%27s_equation#Numerical_approximation_of_inverse_problem
"""

def E(t):

    # difference from epoch [s] (UTC)
    Δt = (t - datetime.strptime(t0, '%Y-%m-%dT%H:%M:%S.%f')).total_seconds()

    # kepler's third law formula for mean anomaly [rad]
    M = M0 + n * Δt

    equation = lambda E: M - E + e * np.sin(E) # rearange the terms so they are equal to 0
    derivative = lambda E: e * np.cos(E) - 1 # it's equal to equation derivative

    E = M # assume eccentric anomaly is equal to mean anomaly
    accuracy = 10 # times to solve for eccentric anomaly (more calculus = more accurate)

    for _ in range(accuracy): E = E - (equation(E) / derivative(E))

    return E


"""
COE To PQW To ECI TO ECR (ECEF)

From Polar To Perifocal:
https://en.wikipedia.org/wiki/True_anomaly#From_the_eccentric_anomaly
https://en.wikipedia.org/wiki/True_anomaly#Radius_from_true_anomaly
https://en.wikipedia.org/wiki/Polar_coordinate_system#Converting_between_polar_and_Cartesian_coordinates

From Perifocal To Geocentric (Using Euler Angles):
https://en.wikipedia.org/wiki/Orbital_elements#Euler_angle_transformations
"The inverse transformation, which computes the 3 coordinates in the X-Y-Z system,
given the 3 (or 2) coordinates in the P-Q-W system, is represented by the inverse matrix."

Visual Representation: https://www.youtube.com/watch?v=fzTJmMnTinU

Faster Methods (PQW TO ECI):
https://modelica.org/events/modelica2008/Proceedings/sessions/session4d1.pdf
https://downloads.rene-schwarz.com/download/M001-Keplerian_Orbit_Elements_to_Cartesian_State_Vectors.pdf
"""

def ECI(t):

    # calculate the semi-major axis from kepler's 3rd law
    a = np.cbrt((G * Me) / np.power(n, 2))
    #b = a * np.sqrt( 1 - np.power(e, 2) ) # semi-minor axis

    # true anomaly [rad] & radius [m]
    ν = 2 * np.arctan(np.sqrt((1 + e) / (1 - e)) * np.tan(E(t) / 2))
    r = (a * (1 - np.power(e, 2))) / (1 + e * np.cos(ν))

    PQW = r * np.array([np.cos(ν), np.sin(ν), 0])

    ECI = (RZ(-Ω) @ RX(-i) @ RZ(-ω)) @ PQW

    return ECI

def ECEF(t):

    # universal time elapsed since j2000 (2451545.0) [s]
    J = (t - datetime(2000, 1, 1, 12, 0, 0)).total_seconds()
    # dynamic time used in astronomy
    T = (J / 86400) / 36525

    # greenwich apparent (true) sidereal time
    Λ = (H(J) + ΔH(T)) % PI2

    ECEF = (W(T) @ RZ(Λ) @ Q(T)) @ ECI(t)

    return ECEF


"""
ECI TO LLH (Latitude Longitude Height)

https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
"If declination measures elevation from the reference plane instead of inclination from the zenith, 
the arccos above becomes an arcsin, and the cos(dec) and sin(dec) below become switched."

https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#Newton%E2%80%93Raphson_method
Newton & Raphson Method - https://en.wikipedia.org/wiki/Newton%27s_method

More About Geodetic Transformations:
https://journals.pan.pl/Content/98324/PDF/art05.pdf
https://www.mygeodesy.id.au/documents/Transforming%20Cartesian%20Coordinates.pdf
"""

def LLH(t):

    X, Y, Z = ECEF(t)

    p = np.sqrt(np.power(X, 2) + np.power(Y, 2))
    δ = np.arctan2(Z, p) # declination ~ geocentric latitude (MAX ± 0.01°)

    # compute the geodetic latitude using bowring's method & newthon-raphson iteration method
    c = lambda K: np.power(p, 2) + (1 - e2) * np.power(Z, 2) * np.power(K, 2)
    equation = lambda K: K - 1 - ((e2 * Ee * K) / np.sqrt(c(K)))
    derivative = lambda K: (np.sqrt(np.power(c(K), 3)) - e2 * Ee * np.power(p, 2)) / np.sqrt(np.power(c(K), 3))

    # first approximation of geodetic latitude
    ϕ = np.arctan(np.tan(δ) * (np.power(Ee, 2) / np.power(Pe, 2)))

    K0 = np.tan(ϕ) * (p / Z) # = 1 / (1 - e2)
    K = K0 # first value of K for the iteration
    for _ in range(10): K = K - (equation(K) / derivative(K))

    # geodetic latitude & longitude [rad]
    ϕ = np.arctan(K * (Z / p))
    λ = np.arctan2(Y, X)

    # ellipsoidal height (!= orthometric height) [m]
    h = (1 / e2) * ((1 / K) - (1 / K0)) * np.sqrt(np.power(p, 2) + np.power(Z, 2) * np.power(K, 2))

    return ϕ, λ, h


"""
ECR TO ENU

WGS84
https://gssc.esa.int/navipedia/index.php/Transformations_between_ECEF_and_ENU_coordinates
https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_ECEF_to_ENU
https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
"""

def ENU(t):

    Xs, Ys, Zs = ECEF(t)

    ϕ, λ = 44.43225, 26.10626 # [deg]
    ϕ, λ = np.radians((ϕ, λ))

    h = 95.0 # [m]

    N = Ee / np.sqrt(1 - e2 * np.power(np.sin(ϕ), 2))

    Xr = (N + h) * np.cos(ϕ) * np.cos(λ)
    Yr = (N + h) * np.cos(ϕ) * np.sin(λ)
    Zr = (N * (1 - e2) + h) * np.sin(ϕ)

    Δ = np.array([Xs - Xr, Ys - Yr, Zs - Zr])

    X, Y, Z = (RX((np.pi / 2) - ϕ) @ RZ((np.pi / 2) + λ)) @ Δ

    # offset azimuth by 90 degrees to point towards north
    A = (np.pi / 2) - (np.arctan(Y / X) if X >= 0 else np.arctan(Y / X) + np.pi)
    E = np.arctan(Z / np.sqrt(np.power(X, 2) + np.power(Y, 2)))
    return A, E


"""
TO DO:
geodetic to geocentric
ut1 & polar motion
orbit perturbations
2000 ecef model?

detailed documentation
code beautification
"""

TOI = datetime.utcnow()
ϕ, λ, _ = np.degrees(LLH(TOI)); _, _, h = LLH(TOI) 
A, E = np.degrees(ENU(TOI))

print(f"Geodetic Latitude: {ϕ}°, Longitude: {λ}°; Orthometric Height: {h * 1e-3} KM\nAzimuth (From North): {A % 360}°, Elevation: {E}°")