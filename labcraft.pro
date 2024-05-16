QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(Qt-QrCodeGenerator.pri)

SOURCES += \
    arduino.cpp \
    client.cpp \
    connection.cpp \
    customsqlmodel.cpp \
    employe.cpp \
    equipement.cpp \
    formation.cpp \
    main.cpp \
    mainwindow.cpp \
    projet.cpp \
    reservation.cpp \
    salle.cpp \
    secdialog.cpp \
    smtp.cpp

HEADERS += \
    arduino.h \
    client.h \
    connection.h \
    customsqlmodel.h \
    employe.h \
    equipement.h \
    formation.h \
    mainwindow.h \
    projet.h \
    reservation.h \
    salle.h \
    secdialog.h \
    smtp.h

FORMS += \
    mainwindow.ui \
    secdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressource.qrc
QT += sql

QT += printsupport
QT += charts
QT += serialport
QT += quick
QT += location
CONFIG += location-plugin-googlemaps


# OpenSSL library path
LIBS += -LC:/Users/rayen/Downloads/openssl-1.0.2t-i386-win32
QT += network
QT += multimedia multimediawidgets

QT += core
DEFINES += SMTP_ENABLED
QT += charts
INCLUDEPATH += $$PWD/models

