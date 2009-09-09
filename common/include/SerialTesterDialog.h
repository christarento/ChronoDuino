/*
 * SerialTesterDialog.h
 *
 *  Created on: 26 août 2009
 *      Author: Christo
 */

#ifndef SERIALTESTERDIALOG_H_
#define SERIALTESTERDIALOG_H_

#include <QDialog>

#include "SerialPortReader.h"
#include "ui_SerialTesterDialog.h"

class SerialThread;

class SerialTesterDialog : public QDialog, public SerialPortReader
{
	Q_OBJECT
public:
	SerialTesterDialog(const QString& a_device, const int a_rate, QWidget* a_parent=NULL);
	virtual ~SerialTesterDialog();

	virtual void processSerialData(const char a_value);

private:
	Ui::SerialTesterDialog m_dialog;
	SerialThread* m_serial_thread;

private slots:
	void instantTest();
};

#endif /* SERIALTESTERDIALOG_H_ */
