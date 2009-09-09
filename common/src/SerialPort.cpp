/*
 * SerialPort.cpp
 *
 *  Created on: 26 août 2009
 *      Author: Christo
 */

#include <QDebug>
#include <QMutex>

#include "SerialPort.h"


SerialPort::SerialPort(const QString& device, const int rate, QObject* a_parent) :
	QIODevice(a_parent),
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

	DWORD access = 0;
	if (mode & QIODevice::ReadOnly)
		access |= GENERIC_READ;
	if (mode & QIODevice::WriteOnly)
		access |= GENERIC_WRITE;

	//Open port handle
	m_port_desc = CreateFile(
			m_device.toStdWString().c_str(),
			access, // access mode
			0,                     // comm devices must be opened w/exclusive-access
			0,                     // no security attrs
			OPEN_EXISTING,         // comm devices must use OPEN_EXISTING
			FILE_ATTRIBUTE_NORMAL,  // overlapped I/O
			NULL);                 // hTemplate must be NULL for comm devices
	if (m_port_desc == INVALID_HANDLE_VALUE)
	{
		qCritical() << "QSerial: Could not open " << m_device;
		return false;
	}


	//Port settings
	DCB com_settings = {0};
	com_settings.BaudRate = (DWORD) m_rate;
	com_settings.ByteSize = 8;
	com_settings.StopBits = ONESTOPBIT;
	com_settings.Parity = NOPARITY;

	if ( !SetCommState(m_port_desc, &com_settings) )
	{
		qCritical() << "Unable to set port configuration";
		return false;
	}

	COMMTIMEOUTS timeout;
	timeout.ReadIntervalTimeout = MAXDWORD;
	timeout.ReadTotalTimeoutConstant = 0;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 0;
	timeout.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(m_port_desc, &timeout);

	//Initialization
	setOpenMode(mode);

	return true;
}

void SerialPort::close()
{
	CloseHandle(m_port_desc);
}

qint64 SerialPort::bytesAvailable() const
{
	DWORD error_flags;
	COMSTAT com_state;

	ClearCommError(m_port_desc, &error_flags, &com_state);
	const DWORD avail_bytes = com_state.cbInQue;

	return (qint64)avail_bytes;
}

qint64 SerialPort::readData(char *data, qint64 maxSize)
{
	DWORD read = 0;
	DWORD error_flags;
	COMSTAT com_state;

	// Find out how much data there actually is to read
	// and don't read any more than that.
	ClearCommError(m_port_desc, &error_flags, &com_state);
	const DWORD real_len = std::min((DWORD)maxSize, com_state.cbInQue);
	const BOOL success = ReadFile(
			m_port_desc,// handle of file to read
			data,// address of buffer that receives data
			real_len,// number of bytes to read
			&read,// address of number of bytes read
			0);// address of structure for data

	if (success == FALSE)
		return 0;

	return (qint64)read;
}

qint64 SerialPort::writeData(const char *data, qint64 maxSize)
{
	DWORD written = 0;

	const BOOL success = WriteFile(
			m_port_desc,
			data,
			(DWORD)maxSize,
			&written,
			0);
	if (success == FALSE)
		return 0;

	return (qint64)written;
}

QStringList SerialPort::listSerialPort()
{
	QStringList port_list;

	HKEY hKeySerialComm;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_QUERY_VALUE, &hKeySerialComm) == ERROR_SUCCESS)
	{
		//Get the max value name and max value lengths
		DWORD valueNameMaxLength;
		DWORD valueDataMaxLength;
		DWORD numValues;
		DWORD res = RegQueryInfoKey(
				hKeySerialComm, 	// key handle
				NULL, 			// buffer for class name
				NULL, 			// size of class string
				NULL, 			// reserved
				NULL,			// number of subkeys
				NULL, 			// longest subkey size
				NULL, 			// longest class string
				&numValues,		// number of values for this key
				&valueNameMaxLength,	// longest value name
				&valueDataMaxLength,	// longest value data
				NULL, 			// security descriptor
				NULL			// last write time
		);

		++valueNameMaxLength;//for NULL terminated end of string

		if (res == ERROR_SUCCESS)
		{
			WCHAR * valueNameBuffer = new WCHAR[valueNameMaxLength];
			BYTE * valueDataBuffer = new BYTE[valueDataMaxLength];
			DWORD dataType;
			for (DWORD index = 0; index < numValues; ++index)
			{
				//Reinit
				memset(valueNameBuffer, 0, valueNameMaxLength*sizeof(WCHAR));
				memset(valueDataBuffer, 0, valueDataMaxLength*sizeof(BYTE));
				res = RegEnumValue(
						hKeySerialComm,// key handle
						index,// value index
						valueNameBuffer,// value name
						&valueNameMaxLength,// size of value name
						NULL,// reserved, must be NULL
						&dataType,// data type
						valueDataBuffer,// value data
						&valueDataMaxLength);
				if( res == ERROR_SUCCESS && dataType == REG_SZ)
					port_list.append(QString::fromUtf16((ushort*)valueDataBuffer));
				else
					qCritical() << "Unable to retrieve registry key value " << res;
			}
			delete[] valueNameBuffer;
			delete[] valueDataBuffer;
		}
		else
			qCritical() << "Unable to retrieve registry info";
	}
	else
		qCritical() << "Unable to open registry KEY";

	//Close handle
	RegCloseKey(hKeySerialComm);

	return port_list;
}
