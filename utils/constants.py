from numpy import pi

"""
                            Constants & Conversions

Earth's Measures - https://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html
Gravitational Constant - https://en.wikipedia.org/wiki/Gravitational_constant#Modern_value
Time Differentiations - https://datacenter.iers.org/data/latestVersion/6_BULLETIN_A_V2013_016.txt
"""

Ee = 6378.137e3 # equatorial radius [m]
Pe = 6356.752e3 # polar radius [m]
Me = 5.97240e24 # mass [kg]
We = 7.2921e-05 # rotation rate [rad/s]

f = 1 - (Pe / Ee); e2 = f * (2 - f) # earth's first eccentricity squared = 6.69437999014e-3

G  = 6.6743e-11 # [m^3 kg^-1 s^-2]

TAI_UTC = 37 # [s]
TT_TAI = 32.184 # [s]

D2S, PI2 = 86400, 2 * pi