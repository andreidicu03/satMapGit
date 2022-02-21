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
    src/TLEParser.cpp \
    src/download.cpp \
    src/gcrs2itrs.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/rotation_matrices.cpp \
    src/sat_track.cpp

HEADERS += \
    src/TLEParser.hpp \
    src/download.hpp \
    src/fileSearch.hpp \
    src/gcrs2itrs.hpp \
    src/includes.hpp \
    src/mainwindow.hpp \
    src/nutation_table.hpp \
    src/rotation_matrices.hpp \
    src/sat_track.hpp

FORMS += \
    ui/mainwindow.ui

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
