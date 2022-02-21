from requests import get

"""
GET Request Satellite Data
https://celestrak.com/NORAD/documentation/gp-data-formats.php
"""

def get_satellite(catnr, format = "JSON"):

    satellite = get("https://celestrak.com/NORAD/elements/gp.php",
    params = {"CATNR": catnr, "FORMAT": format})

    data, = satellite.json()

    """
    'OBJECT_NAME': 'ISS (ZARYA)'
    'OBJECT_ID': '1998-067A'

    'EPOCH': t0 [%Y-%m-%dT%H:%M:%S.%f] (UTC)
    'MEAN_MOTION': n [rev/day]
    'ECCENTRICITY': e [1]
    'INCLINATION': i [deg]
    'RA_OF_ASC_NODE': Ω [deg]
    'ARG_OF_PERICENTER': ω [deg]
    'MEAN_ANOMALY': M0 [deg]

    'EPHEMERIS_TYPE': 0
    'CLASSIFICATION_TYPE': 'U'
    'NORAD_CAT_ID': 25544
    'ELEMENT_SET_NO': 999
    'REV_AT_EPOCH': 30416
    'BSTAR': 9.0971e-06
    'MEAN_MOTION_DOT': 4.9e-07
    'MEAN_MOTION_DDOT': 0
    """

    return data.values()