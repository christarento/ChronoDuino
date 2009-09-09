/*
 * NewResultDialog.h
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#ifndef NEWRESULTDIALOG_H_
#define NEWRESULTDIALOG_H_

#include <QDialog>
#include <QTime>
#include <QTimer>

#include "NewResultThread.h"
#include "SerialPortReader.h"
#include "ui_NewResultDialog.h"


class SerialThread;

class NewResultDialog : public QDialog, public SerialPortReader
{
	Q_OBJECT
public:
	enum Status
	{
		WAITING,
		CONNECTED,
		ARMED,
		RUNNING,
		FINISHED
	};

	NewResultDialog(QWidget* a_parent=NULL);
	virtual ~NewResultDialog();

	virtual void processSerialData(const char a_value);
	void processSocketData(const char a_value);

private:
	void initConnection();
	void initRound();
	void initSerial();

	void sendCompetitorInformations();
	void sendCurrentTime();

	Ui::NewResultDialog m_dialog;
	NewResultThread* m_result_thread;
	SerialThread* m_serial_thread;

	Status m_state;
	QTime m_time;
	QTimer m_refresh_timer;
	int m_elapsed_time;

private slots:
	void start();
	void stop();

	void refreshCompetitor(int a_round_idx);
	void refreshTime();
	void onArm();
	void onQuit();

	void onConnected();
	void onDisconnected();
	void onHostFound();
};

#endif /* NEWRESULTDIALOG_H_ */
