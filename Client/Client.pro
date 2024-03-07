QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    assets/CustomButton.cpp \
    assets/actor.cpp \
    assets/game.cpp \
    assets/platform.cpp \
    chatframe.cpp \
    main.cpp \
    mainwindow.cpp \
    network.cpp

HEADERS += \
    assets/CustomButton.h \
    assets/actor.h \
    assets/game.h \
    assets/platform.h \
    chatframe.h \
    enums.h \
    mainwindow.h \
    network.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
