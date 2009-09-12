/*
 * ServerThread.cpp
 *
 *  Created on: 11 sept 2009
 *      Author: Christo
 */

#include <QStringList>

#include "ServerThread.h"


ServerThread::ServerThread(QObject* a_parent) :
	QThread(a_parent),
	m_socket(NULL),
	m_descriptor(-1),
	m_current_data_type(UNDEFINED),
	m_num_bytes_to_read(-1)
{

}

ServerThread::~ServerThread()
{
	quit();
	wait();
	delete m_socket;
}

void ServerThread::open(const int a_descriptor)
{
	m_descriptor = a_descriptor;
	start();
}

void ServerThread::arm()
{
	if (m_socket)
		m_socket->write("A", 1);
	else
		qCritical("stopChrono(): No socket");
}

void ServerThread::stopChrono()
{
	if (m_socket)
		m_socket->write("F", 1);
	else
		qCritical("stopChrono(): No socket");
}

void ServerThread::run()
{
	m_socket = new QTcpSocket;
	//connect(m_socket, SIGNAL(connected()), SLOT(socketConnected()), Qt::DirectConnection);
	connect(m_socket, SIGNAL(readyRead()), SLOT(processData()), Qt::DirectConnection);
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)), Qt::DirectConnection);

	m_socket->setSocketDescriptor(m_descriptor);
	m_socket->write("C", 1);
	emit connected();
	exec();
}

void ServerThread::processData()
{
	do
	{
		if (m_current_data_type == UNDEFINED)
		{
			if (!readProtocolHeader())
				return;
		}
		if (!hasEnoughData())
			return;
		process();
	}
	while(m_socket->bytesAvailable() > 0);
}

void ServerThread::socketError(QAbstractSocket::SocketError a_error)
{
	qDebug("ServerThread::socketError()");

	switch(a_error)
	{
	default:
		emit error(tr("Connection reset by peer"));
	}
}

void ServerThread::socketConnected()
{
	qDebug("ServerThread::socketConnected()");
	m_socket->write("C", 1);
	emit connected();
}

bool ServerThread::readProtocolHeader()
{
	if (readDataIntoBuffer() <= 0)
		return false;

	if (m_buffer == "CO\\")
		m_current_data_type = COMPETITOR_INFO;
	else if (m_buffer == "IM\\")
		m_current_data_type = COMPETITOR_PHOTO;
	else if (m_buffer == "TI\\")
		m_current_data_type = TIME;
	else
	{
		m_current_data_type = UNDEFINED;
		m_socket->abort();
		return false;
	}

	m_buffer.clear();
	m_num_bytes_to_read = dataLengthForCurrentDataType();
	return true;
}

int ServerThread::readDataIntoBuffer(const int a_max_size)
{
	if (a_max_size > MAX_BUFFER_SIZE)
		return 0;

	const int numBytesBeforeRead = m_buffer.size();
	if (numBytesBeforeRead > MAX_BUFFER_SIZE)
	{
		m_socket->abort();
		return 0;
	}

	while (m_socket->bytesAvailable() > 0 && m_buffer.size() < a_max_size)
	{
		m_buffer.append(m_socket->read(1));
		if (m_buffer.endsWith(SEPARATOR_TOKEN))
			break;
	}
	return m_buffer.size() - numBytesBeforeRead;
}

int ServerThread::dataLengthForCurrentDataType()
{
	if (m_socket->bytesAvailable() <= 0 || readDataIntoBuffer() <= 0 || !m_buffer.endsWith(SEPARATOR_TOKEN))
		return 0;

	m_buffer.chop(1);//remove separator token
	const int number = m_buffer.toInt();
	m_buffer.clear();
	return number;
}

bool ServerThread::hasEnoughData()
{
	if (m_num_bytes_to_read <= 0)
		m_num_bytes_to_read = dataLengthForCurrentDataType();

	if (m_socket->bytesAvailable() < m_num_bytes_to_read || m_num_bytes_to_read <= 0)
		return false;

	return true;
}

void ServerThread::process()
{
	 m_buffer = m_socket->read(m_num_bytes_to_read);
     if (m_buffer.size() != m_num_bytes_to_read)
     {
         m_socket->abort();
         return;
     }

     switch (m_current_data_type)
     {
     case COMPETITOR_INFO:
     {
    	 const QStringList competitor = QString::fromUtf8(m_buffer).split('@', QString::KeepEmptyParts);
    	 if (competitor.size() != 3)
    		 qDebug("competitor mis formed");
    	 else
    	 {
    		 emit competitorArmed(competitor[0], competitor[1], competitor[2]);
    	 }
    	 break;
     }

     case COMPETITOR_PHOTO:
     {
    	 break;
     }

     case TIME:
     {
    	 const int elapsed = m_buffer.toInt();
    	 emit currentTime(elapsed);
    	 break;
     }

     default:
    	 break;
     }

     m_current_data_type = UNDEFINED;
     m_num_bytes_to_read = 0;
     m_buffer.clear();
}
