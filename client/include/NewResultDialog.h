/*
 * NewResultDialog.h
 *
 *  Created on: 15 ao�t 2009
 *      Author: Christo
 */

#ifndef NEWRESULTDIALOG_H_
#define NEWRESULTDIALOG_H_

#include <QDialog>
#include <QSound>

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
		ARMED,
		RUNNING,
		FINISHED
	};

	NewResultDialog(QWidget* a_parent=NULL);
	virtual ~NewResultDialog();

	virtual void processSerialData(const char a_value);

signals:
	void serialInitialized();
	void started();

private:
	void initSerial();
	void initConnection();

	Ui::NewResultDialog m_dialog;
	NewResultThread* m_result_thread;
	SerialThread* m_serial_thread;

	Status m_state;

	QSound* m_armed_sound;
	QSound* m_sensor_sound;
	QSound* m_finished_sound;

private slots:
	void refreshRound();
	void sendCompetitorInformations();
	void armedState();
	void start();

	void refreshCompetitor(int a_round_idx);
	void refreshTime(const int& a_time);
	void stop(const int& a_time);

	void onArm();
	void onQuit();

	void socketError(const QString& a_message);
};

#endif /* NEWRESULTDIALOG_H_ */
