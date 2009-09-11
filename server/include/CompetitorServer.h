/*
 * CompetitorServer.h
 *
 *  Created on: 11 sept. 2009
 *      Author: christo
 */

#ifndef COMPETITORSERVER_H_
#define COMPETITORSERVER_H_

#include <QTcpServer>

class CompetitorServer : public QTcpServer
{
	Q_OBJECT
public:
	CompetitorServer(QObject* a_parent=NULL);
	virtual ~CompetitorServer();

signals:
	void newCompetitor(const int&);

protected:
	virtual void incomingConnection(int socketDescriptor);

private:
};

#endif /* COMPETITORSERVER_H_ */
