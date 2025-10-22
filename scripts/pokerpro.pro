QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Cards.cpp \
    Deck.cpp \
    Network_Controller.cpp \
    Players.cpp \
    Server_Controller.cpp \
    Table.cpp \
    WinningHand.cpp \
    gamewindow.cpp \
    machstate.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Cards.h \
    Deck.h \
    Network_Controller.h \
    Players.h \
    Server_Controller.h \
    Table.h \
    WinningHand.h \
    gamewindow.h \
    machstate.h \
    mainwindow.h

FORMS += \
    gamewindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=
