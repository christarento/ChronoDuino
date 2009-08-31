/*
 * SerialTesterDialog.h
 *
 *  Created on: 26 août 2009
 *      Author: Christo
 */

#ifndef SERIALTESTERDIALOG_H_
#define SERIALTESTERDIALOG_H_

#include <QDialog>

#include "ui_SerialTesterDialog.h"
#include "SerialPort.h"

class SerialTesterDialog : public QDialog
{
	Q_OBJECT
public:
	SerialTesterDialog(const QString& a_device, const int a_rate, QWidget* a_parent=NULL);
	virtual ~SerialTesterDialog();

private:
	Ui::SerialTesterDialog m_dialog;
	SerialPort* m_serial_port;

private slots:
	void processData();
};

#endif /* SERIALTESTERDIALOG_H_ */
