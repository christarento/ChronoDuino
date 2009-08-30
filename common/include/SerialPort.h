/*
 * SerialPort.h
 *
 *  Created on: 26 août 2009
 *      Author: Christo
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <QIODevice>
#include <QStringList>
#include <QTimer>

#include <windows.h>

class SerialPort : public QIODevice
{
	Q_OBJECT

public:
	SerialPort(const QString& device, const int rate, QObject* a_parent=NULL);
	virtual ~SerialPort();


	/**
	 * This method is called automatically by the serial
	 * class and should not be called from outside.
	 */
	bool open(OpenMode mode);

	void setFlowControl(bool value);
	void setKeepOpen(bool value);

	int fd() const {return (int)m_port_desc;}

	/** Closes the serial port. */
	virtual void close();

	/**
	 *
	 */
	virtual bool isSequential() const {return true;}

	/**
	 * Returns the number of bytes available to be read.
	 */
	virtual qint64 bytesAvailable() const;

	/**
	 * Reads max maxlen bytes from the socket into data and returns the
	 * number of bytes read.  Returns -1 if an error
	 * occurred.
	 */
	virtual qint64 readData(char *data, qint64 maxSize);

	/**
	 * Writes len bytes from data and returns the number of
	 * bytes written.  Returns -1 if an error occured.
	 */
	virtual qint64 writeData(const char *data, qint64 maxSize);

	virtual void setErrorString(const QString& str) {}

	/**
	 * Gets list of available serial ports
	 */
	static QStringList listSerialPort();

protected:


private:
	HANDLE m_port_desc;
	OVERLAPPED m_overlapped_io;

	QString m_device;
	int m_rate;

	QTimer* m_timer;

private slots:
	void checkData();
};

#endif /* SERIALPORT_H_ */
