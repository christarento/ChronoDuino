//============================================================================
// Name        : ChronoDuino.cpp
// Author      : Chrisalys.org
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <QApplication>
#include "ChronoDuino.h"
#include "version.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("Chrisalys");
	app.setApplicationName("ChronoDuino");
	app.setApplicationVersion(revision);
	Q_INIT_RESOURCE(chrono);

	ChronoDuino mainWin;
	mainWin.show();

	return app.exec();
}
