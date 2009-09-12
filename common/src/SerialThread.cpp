/*
 * SerialThread.cpp
 *
 *  Created on: 5 sept. 2009
 *      Author: Christo
 */

#include <QDebug>
#include <QSettings>

#include "SerialPortReader.h"
#include "SerialThread.h"


SerialThread::SerialThread(SerialPortReader* a_reader, QObject* a_parent) :
	QThread(a_parent),
	m_reader(a_reader),
	m_serial_port(NULL),
	m_quit(false)
{

}

SerialThread::~SerialThread()
{
	m_quit = true;
	wait();
}

void SerialThread::open(const QString& device, const int rate, const QIODevice::OpenMode mode)
{
	m_device = device;
	m_rate = rate;
	m_mode = mode;

	start(QThread::TimeCriticalPriority);
}

void SerialThread::sendChar(const char& a_value)
{
	if (m_serial_port)
		m_serial_port->writeData(&a_value, 1);
}

void SerialThread::run()
{
	SerialPort serial_port(m_device, m_rate);
	serial_port.open(m_mode);
	if (!serial_port.isOpen())
		return;

	emit connected();

	while (!m_quit)
	{
		while (serial_port.size())
		{
			char value;
			if (serial_port.getChar(&value))
				m_reader->processSerialData(value);
		}
		QThread::usleep(500);
	}

	m_serial_port = NULL;
}
