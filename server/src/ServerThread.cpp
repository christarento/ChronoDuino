/*
 * ServerThread.cpp
 *
 *  Created on: 11 sept 2009
 *      Author: Christo
 */

#include "ServerThread.h"

ServerThread::ServerThread(QObject* a_parent) :
	QThread(a_parent),
	m_quit(false),
	m_socket(NULL),
	m_descriptor(-1)
{

}

ServerThread::~ServerThread()
{
	m_quit = true;
	wait();
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
	QTcpSocket socket;
	socket.setSocketDescriptor(m_descriptor);
	if (!socket.isOpen())
		return;

	m_socket = &socket;
	socket.write("C", 1);

	while (!m_quit)
	{
		QThread::usleep(500);
	}

	m_socket = NULL;
}
