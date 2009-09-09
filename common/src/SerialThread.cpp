/*
 * SerialThread.cpp
 *
 *  Created on: 5 sept. 2009
 *      Author: Christo
 */

#include <QSettings>
#include <QDebug>

#include "SerialPort.h"
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

void SerialThread::run()
{
	m_serial_port = new SerialPort(m_device, m_rate);
	m_serial_port->open(m_mode);
	if (!m_serial_port->isOpen())
		return;

	while (!m_quit)
	{
		if (m_serial_port->size())
		{
			char value;
			if (m_serial_port->getChar(&value))
				m_reader->processSerialData(value);
		}
		QThread::usleep(500);
	}

	delete m_serial_port;
	m_serial_port = NULL;
}
