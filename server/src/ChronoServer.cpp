/*
 * ChronoServer.cpp
 *
 *  Created on: 8 ao�t 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QTime>

#include "ChronoServer.h"
#include "EditPreferencesDialog.h"
#include "SerialTesterDialog.h"

ChronoServer::ChronoServer(QWidget* a_parent) :
	QMainWindow(a_parent),
	m_server_thread(NULL),
	m_serial_thread(NULL),
	m_state(WAITING_FOR_CONNECTION)
{
	//UI
	m_chrono_server.setupUi(this);
	m_chrono_server.m_central_widget->setEnabled(false);

	//TCP Server
	m_server = new CompetitorServer(this);
	connect(m_server, SIGNAL(newCompetitor(const int&)), SLOT(createNewConnection(const int&)));

	//Connect
	connect(m_chrono_server.m_action_about, SIGNAL(triggered()), SLOT(aboutAction()));
	connect(m_chrono_server.m_action_listen, SIGNAL(triggered()), SLOT(listenAction()));
	connect(m_chrono_server.m_action_preferences, SIGNAL(triggered()), SLOT(preferencesAction()));
	connect(m_chrono_server.m_action_test, SIGNAL(triggered()), SLOT(testAction()));
	connect(m_chrono_server.m_pb_arm, SIGNAL(clicked()), SLOT(onArm()));

	connect(this, SIGNAL(serialInitialized()), SLOT());
	connect(this, SIGNAL(finished()), SLOT(reset()));
}

ChronoServer::~ChronoServer()
{

}

void ChronoServer::aboutAction()
{
	QString message = "<qt><b>ChronoServer (" + qApp->applicationVersion() + ")</b><br/>";
	message += tr("<b>ChronoServer</b> is part a complete race chronometer software suite.");

	QMessageBox::about(
			this,
			tr("ChronoServer"),
			message);
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
	//Settings
	const QSettings settings;
	const QString device = settings.value(EditPreferencesDialog::SERIAL_PORT).toString();
	const int rate = settings.value(EditPreferencesDialog::SERIAL_RATE).toInt();

	SerialTesterDialog test_dialog(device, rate, this);
	test_dialog.exec();
}

void ChronoServer::initSerial()
{
	//Settings
	const QSettings settings;
	const QString device = settings.value(EditPreferencesDialog::SERIAL_PORT).toString();
	const int rate = settings.value(EditPreferencesDialog::SERIAL_RATE).toInt();

	m_serial_thread = new SerialThread(this, this);
	m_serial_thread->open(device, rate, QIODevice::ReadWrite);
}

void ChronoServer::onArm()
{
	m_state = ARMED;
	m_server_thread->arm();
}

void ChronoServer::processSerialData(const char a_value)
{
	switch (a_value)
	{
	case 'H':
		if (m_state == RUNNING)//finish
		{
			m_server_thread->stopChrono();
			emit finished();
		}
		break;

	case 'T':
		qDebug("processSerialData T");
		emit serialInitialized();
		break;

	default:
		break;
	}
}

void ChronoServer::createNewConnection(const int& a_descriptor)
{
	//Thread
	m_server_thread = new ServerThread(this);
	connect(m_server_thread, SIGNAL(connected()), SLOT());
	connect(m_server_thread, SIGNAL(error(const QString&)), SLOT());
	connect(m_server_thread,
			SIGNAL(competitorArmed(const QString&, const QString&, const QString&)),
			SLOT(setCompetitorInformations(const QString&, const QString&, const QString&)));
	connect(m_server_thread, SIGNAL(currentTime(const int&)), SLOT(refreshTime(const int&)));

	m_server_thread->open(a_descriptor);
}

void ChronoServer::setCompetitorInformations(
			const QString& a_first_name,
			const QString& a_last_name,
			const QString& a_category)
{
	//UI
	m_chrono_server.m_central_widget->setEnabled(true);

	m_chrono_server.m_lbl_first_name->setText(a_first_name);
	m_chrono_server.m_lbl_last_name->setText(a_last_name);
	m_chrono_server.m_lbl_round->setText(a_category);
}

void ChronoServer::refreshTime(const int& a_time)
{
	QTime time;
	time.addMSecs(a_time);

	//UI
	m_chrono_server.m_lbl_time->setText( time.toString("mm:ss:zzz") );
}

void ChronoServer::reset()
{
	//Reset UI
	m_chrono_server.m_central_widget->setEnabled(false);
	m_chrono_server.m_lbl_first_name->clear();
	m_chrono_server.m_lbl_last_name->clear();
	m_chrono_server.m_lbl_round->clear();
	m_chrono_server.m_lbl_time->setText("00:00:000");

	//Socket
	if (m_server_thread)
	{
		m_server_thread->deleteLater();
		m_server_thread = NULL;
	}

	//Serial
	if (m_serial_thread)
	{
		m_serial_thread->deleteLater();
		m_serial_thread = NULL;
	}

	if (m_state!=FINISHED)
	{
		QMessageBox::critical(this,
				tr("Connection error"),
				tr("Connection closed by peer"));
	}

	m_state = WAITING_FOR_CONNECTION;
}
