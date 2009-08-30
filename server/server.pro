TEMPLATE = app
TARGET = ChronoServer
CONFIG += qt console
DESTDIR = ../bin

QT += network
MOC_DIR = tmp
UI_DIR = include/ui

DEPENDPATH += . include src ui
INCLUDEPATH += . include ../common/include ../common/include/ui
LIBS += -L../lib -lcommon

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