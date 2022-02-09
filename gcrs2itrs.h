#ifndef GCRS2ITRS_H
#define GCRS2ITRS_H

#include "includes.h"
#include "rotation_matrices.h"
#include "nutation_table.h"

inline void N(float T, float &ε, float &Δψ, float &Δε);
inline void P(float T, float &z, float &ϑ, float &ζ);
inline QMatrix3x3 Q(float T);
inline QMatrix3x3 W();
inline float H(float JDt);
inline float ΔH (float T);
#endif // GCRS2ITRS_H
