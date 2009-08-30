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

#include "ui_NewResultDialog.h"

class NewResultDialog : public QDialog
{
	Q_OBJECT
public:
	enum Status
	{
		WAITING_FOR_CONNECTION,
		CONNECTED,
		ARMED,
		RUNNING,
		FINISHED
	};

	NewResultDialog(QWidget* a_parent=NULL);
	virtual ~NewResultDialog();

private:
	void initConnection();
	void initRound();

	void sendCompetitorInformations();
	void sendCurrentTime();

	Ui::NewResultDialog m_dialog;
	QTcpSocket* m_socket;
	Status m_status;
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

	void onConnected();
	void onDisconnected();
	void onHostFound();
	void onReadyRead();
};

#endif /* NEWRESULTDIALOG_H_ */
