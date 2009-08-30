/*
 * main.cpp
 *
 *  Created on: 8 août 2009
 *      Author: Christo
 */

#include <QApplication>
#include "ChronoServer.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("Chrisalys");
	app.setApplicationName("ChronoServer");
	Q_INIT_RESOURCE(chrono);

	ChronoServer mainWin;
	mainWin.show();

	return app.exec();
}
