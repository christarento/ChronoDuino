/*
 * NewResultDialog.h
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#ifndef NEWRESULTDIALOG_H_
#define NEWRESULTDIALOG_H_

#include <QDialog>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

#include "SerialPort.h"
#include "ui_NewResultDialog.h"

class NewResultDialog : public QDialog
{
	Q_OBJECT
public:
	enum Status
	{
		WAITING_FOR_CONNECTION,
		CONNECTED,
		READY,
		ARMED,
		RUNNING,
		FINISHED
	};

	NewResultDialog(QWidget* a_parent=NULL);
	virtual ~NewResultDialog();

private:
	void initConnection();
	void initRound();
	void initSerial();

	void sendCompetitorInformations();
	void sendCurrentTime();

	Ui::NewResultDialog m_dialog;
	QTcpSocket* m_socket;
	SerialPort* m_serial_port;

	Status m_state;
	QTime m_time;
	QTimer* m_refresh_timer;
	int m_elapsed_time;

private slots:
	void start();
	void stop();

	void refreshCompetitor(int a_round_idx);
	void refreshTime();
	void onArm();
	void onQuit();

	void processSerialData();
	void processSocketData();

	void onConnected();
	void onDisconnected();
	void onHostFound();
};

#endif /* NEWRESULTDIALOG_H_ */
