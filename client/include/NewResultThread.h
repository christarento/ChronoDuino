/*
 * NewResultThread.h
 *
 *  Created on: 8 sept. 2009
 *      Author: Christo
 */

#ifndef NEWRESULTTHREAD_H_
#define NEWRESULTTHREAD_H_

#include <QPixMap>
#include <QTcpSocket>
#include <QThread>

class NewResultDialog;

class NewResultThread : public QThread
{
	Q_OBJECT

public:
	NewResultThread(NewResultDialog* a_parent);
	virtual ~NewResultThread();

	void open(const QString& a_hostname, const int a_port);


public slots:
	void sendCompetitorInformations(
			const QString& a_first_name,
			const QString& a_last_name,
			const QPixmap& a_photo,
			const QString& a_category);
	void sendCurrentTime(const int a_time);

protected:
	virtual void run();

private:
	NewResultDialog* m_result_dialog;
	bool m_quit;

	QTcpSocket* m_socket;
	QString m_hostname;
	int m_port;
};

#endif /* NEWRESULTTHREAD_H_ */
