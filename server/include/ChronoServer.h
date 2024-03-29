/*
 * ChronoServer.h
 *
 *  Created on: 8 ao�t 2009
 *      Author: Christo
 */

#ifndef CHRONOSERVER_H_
#define CHRONOSERVER_H_

#include <QTcpServer>

#include "CompetitorServer.h"
#include "SerialPortReader.h"
#include "SerialThread.h"
#include "ServerThread.h"
#include "ui_ChronoServer.h"

class ChronoServer : public QMainWindow, public SerialPortReader
{
	Q_OBJECT

public:
	enum State
	{
		WAITING_FOR_CONNECTION,
		WAITING_FOR_COMPETITOR,
		READY,
		ARMED,
		FINISHED
	};

	ChronoServer(QWidget* a_parent=NULL);
	virtual ~ChronoServer();

	virtual void processSerialData(const char a_value);

signals:
	void finished();

private:
	void initSerial();

	Ui::ChronoServer m_chrono_server;
	CompetitorServer* m_server;

	ServerThread* m_server_thread;
	SerialThread* m_serial_thread;

	State m_state;

private slots:
	void aboutAction();
	void listenAction();
	void preferencesAction();
	void testAction();

	void onArm();

	void createNewConnection(const int& a_socket_descriptor);
	void waitCompetitor();
	void setCompetitorInformations(
			const QString& a_first_name,
			const QString& a_last_name,
			const QString& a_category);
	void stateArmed();
	void refreshTime(const int& a_time);

	void reset();
};

#endif /* CHRONOSERVER_H_ */
