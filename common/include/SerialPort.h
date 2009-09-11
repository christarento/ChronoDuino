/*
 * SerialPort.h
 *
 *  Created on: 26 ao�t 2009
 *      Author: Christo
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <termios.h>

#include <QIODevice>
#include <QStringList>
#include <QTimer>


class SerialPort : public QIODevice
{
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

	int fd() const {return m_port_desc;}

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

	/**
	 * Gets list of available serial ports
	 */
	static QStringList listSerialPort();

protected:


private:
	int m_port_desc;
	termios m_old_port_settings;

	QString m_device;
	int m_rate;
};

#endif /* SERIALPORT_H_ */
