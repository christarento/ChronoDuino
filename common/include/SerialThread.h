/*
 * SerialThread.h
 *
 *  Created on: 5 sept. 2009
 *      Author: Christo
 */

#ifndef SERIALTHREAD_H_
#define SERIALTHREAD_H_

#include <QThread>

#include "SerialPort.h"

class SerialPortReader;

class SerialThread : public QThread
{
	Q_OBJECT
public:
	SerialThread(SerialPortReader* a_reader, QObject* a_parent = NULL);
	virtual ~SerialThread();

	void open(const QString& device, const int rate, const QIODevice::OpenMode mode);
	void sendChar(const char& a_value);

signals:
	void connected();

protected:
	virtual void run();

private:
	SerialPortReader* m_reader;

	SerialPort* m_serial_port;
	QString m_device;
	int m_rate;
	QIODevice::OpenMode m_mode;

	bool m_quit;
};

#endif /* SERIALTHREAD_H_ */
