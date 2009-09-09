/*
 * NewResultThread.cpp
 *
 *  Created on: 8 sept. 2009
 *      Author: Christo
 */

#include "NewResultDialog.h"
#include "NewResultThread.h"

NewResultThread::NewResultThread(NewResultDialog* a_parent) :
	QThread(a_parent),
	m_result_dialog(a_parent),
	m_quit(false)
{

}

NewResultThread::~NewResultThread()
{

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
	if (!m_socket)
		qCritical("Closed socket!");

	const QString info_str = a_first_name + "\\" + a_last_name + "\\" + a_category;
	const QByteArray info_ba = info_str.toUtf8();
	QByteArray data = "COMPETITOR " + QByteArray::number(info_ba.size()) + " " + info_ba;

	m_socket->write(data);
}

void NewResultThread::sendCurrentTime(const int a_time)
{
	if (!m_socket)
		qCritical("Closed socket!");

	const QByteArray time = QString::number(a_time).toUtf8();
	const QByteArray data = "TIME " + QByteArray::number(time.size()) + " " + time;

	m_socket->write(data);
}

void NewResultThread::run()
{
	//open socket
	m_socket = new QTcpSocket();
	m_socket->connectToHost(m_hostname, m_port, QIODevice::ReadWrite);
	if (!m_socket->waitForConnected())
		m_quit = true;

	//Wait for


	delete m_socket;
	m_socket = NULL;
}
