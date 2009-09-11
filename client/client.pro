TEMPLATE = app
TARGET = ChronoDuino
CONFIG += qt console
DESTDIR = ../bin

QT += network sql
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
HEADERS += include/ChronoContext.h \
	include/ChronoDuino.h \
	include/CompetitorInspector.h \
	include/EditPreferencesDialog.h \
	include/FilterWidget.h \
	include/NewCategoryDialog.h \
	include/NewCompetitorDialog.h \
	include/NewPersonDialog.h \
	include/NewRaceDialog.h \
	include/NewResultDialog.h \
	include/NewResultThread.h \
	include/NewRoundDialog.h \
	include/OpenRaceDialog.h \
	include/PrintDialog.h \
	include/ResultInspector.h \
	include/RoundInspector.h
	
FORMS += src/ui/ChronoDuino.ui \
	src/ui/CompetitorInspector.ui \
	src/ui/EditPreferencesDialog.ui \
	src/ui/FilterWidget.ui \
	src/ui/NewCategoryDialog.ui \
	src/ui/NewCompetitorDialog.ui \
	src/ui/NewPersonDialog.ui \
	src/ui/NewRaceDialog.ui \
	src/ui/NewResultDialog.ui \
	src/ui/NewRoundDialog.ui \
	src/ui/OpenRaceDialog.ui \
	src/ui/PrintDialog.ui \
	src/ui/ResultInspector.ui \
	src/ui/RoundInspector.ui
	
SOURCES += main.cpp \
	src/ChronoContext.cpp \
	src/ChronoDuino.cpp \
	src/CompetitorInspector.cpp \
	src/EditPreferencesDialog.cpp \
	src/FilterWidget.cpp \
	src/NewCategoryDialog.cpp \
	src/NewCompetitorDialog.cpp \
	src/NewPersonDialog.cpp \
	src/NewRaceDialog.cpp \
	src/NewResultDialog.cpp \
	src/NewResultThread.cpp \
	src/NewRoundDialog.cpp \
	src/OpenRaceDialog.cpp \
	src/PrintDialog.cpp \
	src/ResultInspector.cpp \
	src/RoundInspector.cpp

TRANSLATIONS += client_fr.ts

target.path = $$(HOME)/chrono
INSTALLS += target
