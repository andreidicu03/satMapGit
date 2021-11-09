#ifndef ROTATION_MATRICES_H
#define ROTATION_MATRICES_H

#include <cmath>
#include <QGenericMatrix>

void angleSinCos(float n, float& S, float& C);
QMatrix3x3 RotateX(float n);
QMatrix3x3 RotateY(float n);
QMatrix3x3 RotateZ(float n);

#endif // ROTATION_MATRICES_H
