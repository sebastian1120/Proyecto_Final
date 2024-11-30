QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arma.cpp \
    enemigo.cpp \
    main.cpp \
    mainwindow.cpp \
    mancha.cpp \
    personaje.cpp \
    potenciador.cpp \
    protagonista.cpp

HEADERS += \
    arma.h \
    enemigo.h \
    mainwindow.h \
    mancha.h \
    personaje.h \
    potenciador.h \
    protagonista.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rcs.qrc

DISTFILES +=
