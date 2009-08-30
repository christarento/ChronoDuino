/*
 * SerialTesterDialog.cpp
 *
 *  Created on: 26 août 2009
 *      Author: Christo
 */

#include "SerialTesterDialog.h"


SerialTesterDialog::SerialTesterDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	//UI
	m_dialog.setupUi(this);

	//Serial port
	m_serial_port = new SerialPort("COM4", 9600, this);

	//Connect
	connect(m_serial_port, SIGNAL(readyRead()), SLOT(processData()));

	m_serial_port->open(QIODevice::ReadOnly);
}

SerialTesterDialog::~SerialTesterDialog()
{

}

void SerialTesterDialog::processData()
{
	char value;
	if (m_serial_port->getChar(&value))
	{
		switch (value)
		{
		case 'H':
		{
			QPixmap pixmap(":/common/check.png");
			m_dialog.m_lbl->setPixmap(pixmap);
			QApplication::beep();
			break;
		}

		case 'L':
		{
			QPixmap pixmap(":/common/cross.png");
			m_dialog.m_lbl->setPixmap(pixmap);
			break;
		}

		default:
			break;
		}
	}
}
