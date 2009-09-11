/*
 * SerialPort.cpp
 *
 *  Created on: 26 aout 2009
 *      Author: Christo
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <QDebug>
#include <QDir>
#include <QMutex>

#include "SerialPort.h"


SerialPort::SerialPort(const QString& device, const int rate, QObject* a_parent) :
	QIODevice(a_parent),
	m_port_desc(0),
	m_device(device),
	m_rate(rate)
{

}

SerialPort::~SerialPort()
{
	close();
}

bool SerialPort::open(OpenMode mode)
{
	if ( isOpen() )
	{
		qWarning() << "QSerial: already open.";
		return false;
	}

	int access = O_RDONLY;
	if (mode == QIODevice::ReadOnly)
		access = O_RDONLY;
	else if (mode == QIODevice::WriteOnly)
		access = O_WRONLY;
	else if (mode == QIODevice::ReadWrite)
		access = O_RDWR;
	else
	{
		qCritical() << "SerialPort: OpenMode unsupported";
		return false;
	}


	//Open port handle
	m_port_desc = ::open(
			m_device.toStdString().c_str(),
			access | O_NOCTTY | O_NDELAY);// access mode
	if (m_port_desc < 0)
	{
		qCritical() << "SerialPort: Could not open " << m_device;
		return false;
	}


	//Port settings
	if (tcgetattr(m_port_desc, &m_old_port_settings) < 0)
		return false;

	termios new_options;
	bzero(&new_options, sizeof(new_options));

	//Baud rate
	cfsetispeed(&new_options, B9600);//TODO
	cfsetospeed(&new_options, B9600);//TODO

	//Non blocking read
	fcntl(m_port_desc, F_SETFL, FNDELAY);

	//Channel parameters
	new_options.c_cflag |= CREAD | CLOCAL;
	new_options.c_cflag &= ~PARENB;
	new_options.c_cflag &= ~CSTOPB;
	new_options.c_cflag &= ~CSIZE;
	new_options.c_cflag |= CS8;


	//Flush input buffer
	if (tcflush(m_port_desc, TCIFLUSH) < 0)
		return false;

	//Write new settings
	if (tcsetattr(m_port_desc, TCSANOW, &new_options) < 0)
		return false;


	//Initialization
	setOpenMode(mode);

	return true;
}

void SerialPort::close()
{
	tcsetattr(m_port_desc, TCSANOW, &m_old_port_settings);
	::close(m_port_desc);
}

qint64 SerialPort::bytesAvailable() const
{
	int avail_bytes = 0;
	if (ioctl(m_port_desc, FIONREAD, &avail_bytes) < 0)
		return 0;

	return (qint64)avail_bytes;
}

qint64 SerialPort::readData(char *data, qint64 maxSize)
{
	const int read_data = ::read(m_port_desc, data, maxSize);
	if (read_data == -1)
		return 0;

	return (qint64)read_data;
}

qint64 SerialPort::writeData(const char *data, qint64 maxSize)
{
	const int written = ::write(m_port_desc, data, maxSize);
	if (written == -1)
		return 0;

	return (qint64)written;
}

QStringList SerialPort::listSerialPort()
{
	QStringList devices;
	const QDir device_dir("/dev");
	const QStringList filters("ttyUSB*");

	QStringList files = device_dir.entryList(filters, QDir::System);
	QStringListIterator it(files);
	while (it.hasNext())
		devices.append(device_dir.absoluteFilePath(it.next()));

	return devices;
}
