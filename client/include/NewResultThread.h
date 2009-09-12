/*
 * NewResultThread.h
 *
 *  Created on: 8 sept. 2009
 *      Author: Christo
 */

#ifndef NEWRESULTTHREAD_H_
#define NEWRESULTTHREAD_H_

#include <QPixmap>
#include <QTcpSocket>
#include <QThread>
#include <QTime>
#include <QTimer>

class NewResultThread : public QThread
{
	Q_OBJECT

public:
	NewResultThread(QObject* a_parent);
	virtual ~NewResultThread();

	void open(const QString& a_hostname, const int a_port);
	void sendCompetitorInformations(
			const QString& a_first_name,
			const QString& a_last_name,
			const QPixmap& a_photo,
			const QString& a_category);
	void startChrono();

signals:
	void connected();
	void armed();
	void error(const QString&);
	void currentTime(const int&);
	void finished(const int&);

protected:
	virtual void run();

private:
	void stopChrono();

	//Chrono
	QTime m_time;
	int m_elapsed_time;

	//Socket
	QTcpSocket* m_socket;
	QString m_hostname;
	int m_port;

	QTimer* m_refresh_timer;

private slots:
	void refreshTime();
	void processData();
	void socketError(QAbstractSocket::SocketError);
};

#endif /* NEWRESULTTHREAD_H_ */
