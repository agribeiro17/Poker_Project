QT += testlib
QT += gui network core
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_pokerpro.cpp ../../Table.cpp ../../Deck.cpp ../../Players.cpp ../../Cards.cpp ../../Network_Controller.cpp ../../Server_Controller.cpp

HEADERS += ../../Table.h ../../Deck.h ../../Players.h ../../Cards.h ../../Network_Controller.h ../../Server_Controller.h
