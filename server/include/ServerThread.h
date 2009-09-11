/*
 * ServerThread.h
 *
 *  Created on: 11 sept. 2009
 *      Author: christo
 */

#ifndef SERVERTHREAD_H_
#define SERVERTHREAD_H_

#include <QTcpSocket>
#include <QThread>

class ServerThread : public QThread
{
	Q_OBJECT
public:
	ServerThread(QObject* a_parent=NULL);
	virtual ~ServerThread();

	void open(const int a_descriptor);
	void arm();
	void stopChrono();

signals:
	void connected();
	void error(const QString&);

	void competitorArmed(
			const QString& a_first_name,
			const QString& a_last_name,
			const QString& a_category);
	void currentTime(const int&);

protected:
	virtual void run();

private:
	bool m_quit;

	QTcpSocket* m_socket;
	int m_descriptor;
};

#endif /* SERVERTHREAD_H_ */
