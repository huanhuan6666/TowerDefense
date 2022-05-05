QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common.cpp \
    endwindow.cpp \
    enemy.cpp \
    enemybuff.cpp \
    enemynear.cpp \
    enemyremote.cpp \
    enemysuper.cpp \
    flyafraid.cpp \
    flybrave.cpp \
    gamewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp \
    scientist.cpp \
    spikeweed.cpp \
    torchwood.cpp \
    tower.cpp \
    towernut.cpp \
    towerremote.cpp \
    towersuper.cpp

HEADERS += \
    common.h \
    endwindow.h \
    enemy.h \
    enemybuff.h \
    enemynear.h \
    enemyremote.h \
    enemysuper.h \
    flyafraid.h \
    flybrave.h \
    gamewindow.h \
    mainwindow.h \
    map.h \
    scientist.h \
    spikeweed.h \
    torchwood.h \
    tower.h \
    towernut.h \
    towerremote.h \
    towersuper.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
