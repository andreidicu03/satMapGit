from numpy import array, sin, cos

def T(n):
    S = sin(n)
    C = cos(n)

    return S, C

def RX(n):
    S, C = T(n)

    RX = array([
        [ 1, 0, 0 ],
        [ 0, C, S ],
        [ 0,-S, C ]])

    return RX

def RY(n):
    S, C = T(n)

    RY = array([
        [ C, 0,-S ],
        [ 0, 1, 0 ],
        [ S, 0, C ]])

    return RY

def RZ(n):
    S, C = T(n)

    RZ = array([
        [ C, S, 0 ],
        [-S, C, 0 ],
        [ 0, 0, 1 ]])

    return RZ