/*
 * SerialTesterDialog.cpp
 *
 *  Created on: 26 août 2009
 *      Author: Christo
 */

#include <QPushButton>
#include <QMessageBox>

#include "SerialPort.h"
#include "SerialTesterDialog.h"
#include "SerialThread.h"


SerialTesterDialog::SerialTesterDialog(const QString& a_device, const int a_rate, QWidget* a_parent) :
	QDialog(a_parent)
{
	//UI
	m_dialog.setupUi(this);

	//Serial port
	m_serial_thread = new SerialThread(this, this);
	m_serial_thread->open(a_device, a_rate, QIODevice::ReadWrite);

	//Connect
	connect(m_dialog.m_pb_test, SIGNAL(clicked()), SLOT(instantTest()));
}

SerialTesterDialog::~SerialTesterDialog()
{

}

void SerialTesterDialog::processSerialData(const char a_value)
{
	switch (a_value)
	{
	case 'H':
	{
		QApplication::beep();
		break;
	}

	case 'L':
		break;

	case 'T':
	{
		QApplication::beep();
		QPixmap pixmap(":/common/check.png");
		m_dialog.m_lbl->setPixmap(pixmap);
		m_dialog.m_group_box->setEnabled(true);
		break;
	}

	default:
		break;
	}
}

void SerialTesterDialog::instantTest()
{
	const char value = 'T';
	m_serial_thread->getSerialPort()->writeData(&value, 1);
}
