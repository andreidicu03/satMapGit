#ifndef ROTATION_MATRICES_H
#define ROTATION_MATRICES_H

#include <cmath>
#include <QGenericMatrix>

void angleSinCos(float n, float& S, float& C){
    S=sin(n);
    C=cos(n);
}

QMatrix3x3 RotateX(float n){
    float S, C;
    angleSinCos(n, S, C);

    float val []={ 1, 0, 0 ,
                   0, C, S ,
                   0,-S, C };

   QMatrix3x3 RX(val);

   return RX;
}

QMatrix3x3 RotateY(float n){
    float S, C;
    angleSinCos(n, S, C);

    float val []={C, 0, -S,
                  0, 1,  0,
                  S, 0,  C};

    QMatrix3x3 RY(val);

    return RY;
}

QMatrix3x3 RotateZ(float n){
    float S, C;
    angleSinCos(n, S, C);

    float val []={  C, S, 0,
                   -S, C, 0,
                    0, 0, 1};

    QMatrix3x3 RZ(val);

    return RZ;
}
#endif // ROTATION_MATRICES_H
