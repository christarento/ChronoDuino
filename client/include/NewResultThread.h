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
	void startChrono();

signals:
	void connected();
	void armed();
	void error(const QString&);
	void currentTime(const int& a_time);
	void finished(const int& a_time);

public slots:
	void sendCompetitorInformations(
			const QString& a_first_name,
			const QString& a_last_name,
			const QPixmap& a_photo,
			const QString& a_category);

protected:
	virtual void run();

private:
	void processData(const char& a_value);
	void stopChrono();

	//Chrono
	QTime m_time;
	int m_elapsed_time;
	QTimer* m_refresh_timer;

	bool m_quit;

	//Socket
	QTcpSocket* m_socket;
	QString m_hostname;
	int m_port;

private slots:
	void refreshTime();
};

#endif /* NEWRESULTTHREAD_H_ */
