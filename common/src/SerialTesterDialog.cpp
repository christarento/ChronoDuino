/*
 * SerialTesterDialog.cpp
 *
 *  Created on: 26 ao�t 2009
 *      Author: Christo
 */

#include <QDir>
#include <QPushButton>
#include <QMessageBox>
#include <QSound>

#include "SerialPort.h"
#include "SerialTesterDialog.h"
#include "SerialThread.h"


SerialTesterDialog::SerialTesterDialog(const QString& a_device, const int a_rate, QWidget* a_parent) :
	QDialog(a_parent)
{
	//UI
	m_dialog.setupUi(this);

	//Sound
	const QDir sound_dir(qApp->applicationDirPath() + "/sounds");
	m_sound = new QSound(sound_dir.filePath("sensor.wav"), this);

	//Serial port
	m_thread = new SerialThread(this, this);
	connect(m_thread, SIGNAL(connected()), SLOT(changePixmap()));
	m_thread->open(a_device, a_rate, QIODevice::ReadWrite);

	//Connect
	connect(this, SIGNAL(sensor()), m_sound, SLOT(play()));
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
		emit sensor();
		break;
	}
	}
}

void SerialTesterDialog::changePixmap()
{
	QPixmap pixmap(":/common/check.png");
	m_dialog.m_lbl->setPixmap(pixmap);
	m_dialog.m_group_box->setEnabled(true);
}
