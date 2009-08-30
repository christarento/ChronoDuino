/*
 * ChronoServer.cpp
 *
 *  Created on: 8 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QTime>

#include "ChronoServer.h"
#include "EditPreferencesDialog.h"

ChronoServer::ChronoServer(QWidget* a_parent) :
	QMainWindow(a_parent),
	m_socket(NULL),
	m_state(WAITING_FOR_CONNECTION),
	m_current_data_type(UNDEFINED),
	m_num_bytes_to_read(-1)
{
	m_chrono_server.setupUi(this);
	m_chrono_server.m_central_widget->setEnabled(false);

	//Server
	m_server = new QTcpServer(this);
	m_server->setMaxPendingConnections(1);
	connect(m_server, SIGNAL(newConnection()), SLOT(createNewConnection()));

	//Connect
	connect(m_chrono_server.m_action_about, SIGNAL(triggered()), SLOT(aboutAction()));
	connect(m_chrono_server.m_action_listen, SIGNAL(triggered()), SLOT(listenAction()));
	connect(m_chrono_server.m_action_preferences, SIGNAL(triggered()), SLOT(preferencesAction()));
	connect(m_chrono_server.m_action_test, SIGNAL(triggered()), SLOT(testAction()));
}

ChronoServer::~ChronoServer()
{

}

void ChronoServer::aboutAction()
{
	QMessageBox::about(
			this,
			tr("ChronoServer"),
			tr("Server for ChronoDuino"));
}

void ChronoServer::listenAction()
{
	//QSettings
	const QSettings settings;

	//Bind parameters
	const int port = settings.value(EditPreferencesDialog::BIND_PORT, 8000).toInt();

	m_server->listen(QHostAddress::Any, port);
	if (m_server->isListening())
	{
		m_chrono_server.m_action_listen->setEnabled(false);
		m_chrono_server.m_status_bar->showMessage(tr("Listening on port %1").arg(m_server->serverPort()), 5000);
	}
	else
		QMessageBox::critical(
				this,
				tr("Server error"),
				tr("Unable to listen on port <b>%1</b>").arg(port));
}

void ChronoServer::preferencesAction()
{
	EditPreferencesDialog preferences_dialog;
	if (preferences_dialog.exec() == QDialog::Accepted)
		m_chrono_server.m_status_bar->showMessage(tr("Preferences saved."), 5000);
}

void ChronoServer::testAction()
{

}

void ChronoServer::sendMessage(const QString& a_message)
{

}

void ChronoServer::createNewConnection()
{
	m_socket = m_server->nextPendingConnection();
	if (!m_socket)
	{
		m_chrono_server.m_status_bar->showMessage(tr("New connection : socket creation error"), 2000);
		return;
	}

	m_state = WAITING_FOR_COMPETITOR;
	m_chrono_server.m_status_bar->showMessage(
			tr("Connected to %1 on port %2").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort()),
			2000);

	//Connect
	connect(m_socket, SIGNAL(disconnected()), SLOT(reset()));
	connect(m_socket, SIGNAL(readyRead()), SLOT(processReadyRead()));

	//UI
	m_chrono_server.m_central_widget->setEnabled(true);
}

void ChronoServer::reset()
{
	m_socket->deleteLater();
	m_socket = NULL;

	if (m_state!=FINISHED)
	{
		m_chrono_server.m_central_widget->setEnabled(false);
		QMessageBox::critical(this,
				tr("Connection error"),
				tr("Connection closed by peer"));
	}

	m_state = WAITING_FOR_CONNECTION;

	//Reset UI
	m_chrono_server.m_lbl_first_name->clear();
	m_chrono_server.m_lbl_last_name->clear();
	m_chrono_server.m_lbl_round->clear();
	m_chrono_server.m_lbl_time->setText("00:00:000");
}

void ChronoServer::processReadyRead()
{
	if (m_state == WAITING_FOR_COMPETITOR)
	{
		if (!readProtocolHeader())
			return;
		if (m_current_data_type != COMPETITOR_INFO)
		{
			m_socket->abort();
			return;
		}
		m_state = READY;
	}

	if (m_state == READY)
	{
		if (readProtocolHeader())
		{
			m_socket->abort();
			return;
		}
	}

	do
	{
		if (m_current_data_type == UNDEFINED)
		{
			if (!readProtocolHeader())
				return;
		}
		if (!hasEnoughData())
			return;
		processData();
	}
	while(m_socket->bytesAvailable() > 0);
}

bool ChronoServer::readProtocolHeader()
{
	if (readDataIntoBuffer() <= 0)
		return false;

	if (m_buffer == "INFOS ")
		m_current_data_type = COMPETITOR_INFO;
	else if (m_buffer == "PHOTO ")
		m_current_data_type = COMPETITOR_PHOTO;
	else if (m_buffer == "TIME ")
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

int ChronoServer::readDataIntoBuffer(const int a_max_size)
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

int ChronoServer::dataLengthForCurrentDataType()
{
	if (m_socket->bytesAvailable() <= 0 || readDataIntoBuffer() <= 0 || !m_buffer.endsWith(SEPARATOR_TOKEN))
		return 0;

	m_buffer.chop(1);//remove separator token
	const int number = m_buffer.toInt();
	m_buffer.clear();
	return number;
}

bool ChronoServer::hasEnoughData()
{
	if (m_num_bytes_to_read <= 0)
		m_num_bytes_to_read = dataLengthForCurrentDataType();

	if (m_socket->bytesAvailable() < m_num_bytes_to_read || m_num_bytes_to_read <= 0)
		return false;

	return true;
}

void ChronoServer::processData()
{
	 m_buffer = m_socket->read(m_num_bytes_to_read);
     if (m_buffer.size() != m_num_bytes_to_read)
     {
         m_socket->abort();
         return;
     }

     switch (m_current_data_type)
     {
     case TIME:
     {
    	 const int msecs = m_buffer.toInt();
    	 QTime time;
    	 time.addMSecs(msecs);
    	 m_chrono_server.m_lbl_time->setText( time.toString("mm:ss:zzz") );
    	 break;
     }

     default:
    	 break;
     }
}

