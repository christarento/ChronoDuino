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
           include/EditPreferencesDialog.h

FORMS += src/ui/ChronoServer.ui \
         src/ui/EditPreferencesDialog.ui

SOURCES += main.cpp \
           src/ChronoServer.cpp \
           src/EditPreferencesDialog.cpp

RC_FILE = server.rc

TRANSLATIONS += server_fr.ts