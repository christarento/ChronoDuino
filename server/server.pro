TEMPLATE = app
TARGET = ChronoServer
CONFIG += qt console
DESTDIR = ../bin

QT += network
MOC_DIR = tmp
UI_DIR = include/ui

DEPENDPATH += . \
              include \
              include/ui \
              ../common/include \
              ../common/include/ui
INCLUDEPATH += . \
               include \
               include/ui \
               ../common/include \
               ../common/include/ui
               
LIBS += ../lib/libcommon.a
POST_TARGETDEPS += ../lib/libcommon.a

# Input
HEADERS += include/ChronoServer.h \
           include/CompetitorServer.h \
           include/EditPreferencesDialog.h \
           include/ServerThread.h

FORMS += src/ui/ChronoServer.ui \
         src/ui/EditPreferencesDialog.ui

SOURCES += main.cpp \
           src/ChronoServer.cpp \
           src/CompetitorServer.cpp \
           src/EditPreferencesDialog.cpp \
           src/ServerThread.cpp

TRANSLATIONS += server_fr.ts

target.path = $$(HOME)/chrono
INSTALLS += target
