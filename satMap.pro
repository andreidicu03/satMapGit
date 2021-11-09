QT += core gui
QT += network
QT += widgets
QT += location
QT += Marble

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TLEParser.cpp \
    download.cpp \
    gcrs2itrs.cpp \
    main.cpp \
    mainwindow.cpp \
    rotation_matrices.cpp \
    sat_track.cpp

HEADERS += \
    TLEParser.h \
    download.h \
    fileSearch.h \
    gcrs2itrs.h \
    includes.h \
    mainwindow.h \
    nutation_table.h \
    rotation_matrices.h \
    sat_track.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix {
    LIBS += -L/usr/lib/ \
    -lmarblewidget-qt5
    INCLUDEPATH += /usr/include/marble
    DEPENDPATH += /usr/include/marble
}

DISTFILES +=
