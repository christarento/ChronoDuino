/*
 * CompetitorServer.cpp
 *
 *  Created on: 11 sept. 2009
 *      Author: christo
 */

#include "CompetitorServer.h"

CompetitorServer::CompetitorServer(QObject* a_parent) :
	QTcpServer(a_parent)
{

}

CompetitorServer::~CompetitorServer()
{

}

void CompetitorServer::incomingConnection(int socketDescriptor)
{
	emit newCompetitor(socketDescriptor);
}
