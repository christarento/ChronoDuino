/*
 * NewResultThread.cpp
 *
 *  Created on: 8 sept. 2009
 *      Author: Christo
 */

#include "NewResultThread.h"

NewResultThread::NewResultThread(QObject* a_parent) :
	QThread(a_parent),
	m_elapsed_time(-1),
	m_socket(NULL),
	m_port(-1)
{
	m_refresh_timer = new QTimer(this);
	m_refresh_timer->setInterval(500);
	connect(m_refresh_timer, SIGNAL(timeout()), SLOT(refreshTime()));
}

NewResultThread::~NewResultThread()
{
	quit();
	wait();
	delete m_socket;
}

void NewResultThread::open(const QString& a_hostname, const int a_port)
{
	m_hostname = a_hostname;
	m_port = a_port;

	start();
}

void NewResultThread::sendCompetitorInformations(
		const QString& a_first_name,
		const QString& a_last_name,
		const QPixmap& a_photo,
		const QString& a_category)
{
	qDebug("NewResultThread::sendCompetitorInformations");

	if (!m_socket)
		qCritical("Closed socket!");

	const QString info_str = a_first_name + "@" + a_last_name + "@" + a_category;
	const QByteArray info_ba = info_str.toUtf8();
	QByteArray data = "CO\\" + QByteArray::number(info_ba.size()) + "\\" + info_ba;

	m_socket->write(data);
}

void NewResultThread::run()
{
	//open socket
	m_socket = new QTcpSocket;
	connect(m_socket, SIGNAL(readyRead()), SLOT(processData()), Qt::DirectConnection);
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)), Qt::DirectConnection);

	m_socket->connectToHost(m_hostname, m_port, QIODevice::ReadWrite);
	exec();
}

void NewResultThread::processData()
{
	while (m_socket->bytesAvailable())
	{
		char value;
		if (m_socket->read(&value, 1) != 1 )
			continue;

		switch (value)
		{
		case 'C'://connected to host
			qDebug("Process Socket connected");
			emit connected();
			break;

		case 'A'://host armed
			qDebug("Process Socket armed");
			emit armed();
			break;

		case 'F'://finished
			stopChrono();
			break;

		default:
			break;
		}
	}
}

void NewResultThread::socketError(QAbstractSocket::SocketError a_error)
{
	switch(a_error)
	{
	default:
		emit (tr("Connection reset by peer"));
	}
}

void NewResultThread::startChrono()
{
	m_time.start();
	m_elapsed_time = 0;

	m_refresh_timer->start();
}

void NewResultThread::stopChrono()
{
	m_elapsed_time = m_time.elapsed();
	m_refresh_timer->stop();

	emit finished(m_elapsed_time);
}

void NewResultThread::refreshTime()
{
	const int elapsed = m_time.elapsed();

	//Socket
	if (m_socket)
	{
		const QByteArray time = QString::number(elapsed).toUtf8();
		const QByteArray data = "TI\\" + QByteArray::number(time.size()) + "\\" + time;

		m_socket->write(data);
	}
	else
		qCritical("Closed socket!");

	//GUI
	emit currentTime(elapsed);
}
