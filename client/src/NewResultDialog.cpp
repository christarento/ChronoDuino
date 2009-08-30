/*
 * NewResultDialog.cpp
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */


#include <QMessageBox>
#include <QSettings>
#include <QSqlQuery>

#include "ChronoContext.h"
#include "EditPreferencesDialog.h"
#include "NewResultDialog.h"

NewResultDialog::NewResultDialog(QWidget* a_parent) :
	QDialog(a_parent),
	m_socket(NULL),
	m_serial_port(NULL),
	m_state(WAITING_FOR_CONNECTION),
	m_elapsed_time(-1)
{
	//UI
	m_dialog.setupUi(this);
	m_dialog.m_central_widget->setEnabled(false);

	//RefreshTimer
	m_refresh_timer = new QTimer(this);
	m_refresh_timer->setInterval(200);

	//Connect
	connect(m_dialog.m_pb_quit, SIGNAL(clicked()), SLOT(onQuit()));
	connect(m_dialog.m_cb_round, SIGNAL(currentIndexChanged(int)), SLOT(refreshCompetitor(int)));
	connect(m_dialog.m_pb_arm, SIGNAL(clicked()), SLOT(onArm()));
	connect(m_refresh_timer, SIGNAL(timeout()), SLOT(refreshTime()));

	initSerial();
	initConnection();
	initRound();
}

NewResultDialog::~NewResultDialog()
{

}

void NewResultDialog::initConnection()
{
	//Socket
	m_socket = new QTcpSocket(this);
	connect(m_socket, SIGNAL(connected()), SLOT(onConnected()));
	connect(m_socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
	connect(m_socket, SIGNAL(hostFound()), SLOT(onHostFound()));
	connect(m_socket, SIGNAL(readyRead()), SLOT(processSocketData()));

	//Settings
	const QSettings settings;
	const QString hostname = settings.value(EditPreferencesDialog::SERVER_URL).toString();
	const int port = settings.value(EditPreferencesDialog::SERVER_PORT).toInt();

	//Connection
	m_socket->connectToHost(hostname, port, QIODevice::ReadWrite);
}

void NewResultDialog::initRound()
{
	m_dialog.m_cb_round->clear();
	m_dialog.m_cb_competitor->clear();

	const int race_id = CHRONOCONTEXT.getCurrentRace();

	//Query
	QSqlQuery query;
	query.prepare("SELECT id, name from t_rounds "
			"WHERE race_id=:race_id");
	query.bindValue(":race_id", race_id);
	query.exec();

	while (query.next())
		m_dialog.m_cb_round->addItem(query.value(1).toString(), query.value(0).toInt());
}

void NewResultDialog::initSerial()
{
	//Settings
	const QSettings settings;
	const QString device = settings.value(EditPreferencesDialog::SERIAL_PORT).toString();
	const int rate = settings.value(EditPreferencesDialog::SERIAL_RATE).toInt();

	//Serial port
	m_serial_port = new SerialPort(device, rate, this);
	connect(m_serial_port, SIGNAL(readyRead()), SLOT(processSerialData()));

	//Connection
	m_serial_port->open(QIODevice::ReadWrite);
}

void NewResultDialog::sendCompetitorInformations()
{
	const int registration_idx = m_dialog.m_cb_competitor->currentIndex();
	const int registration_id = m_dialog.m_cb_competitor->itemData(registration_idx).toInt();
	QSqlQuery query;
	query.prepare("SELECT per.first_name, per.last_name, per.photo FROM t_registrations reg"
			"JOIN t_persons per ON reg.person_id=per.id "
			"WHERE reg.id=:registration_id");
	query.bindValue(":registration_id", registration_id);
	query.exec();

	if (!query.next())
		return;

	QString info_str = query.value(0).toString() + "\\" + query.value(1).toString();
	QByteArray info_ba = info_str.toUtf8();
	QByteArray data = "COMPETITOR " + QByteArray::number(info_ba.size()) + " " + info_ba;
	if (m_socket->write(data) == data.size())
	 ;//isGreetingMessageSent = true;

	//TODO photo
}

void NewResultDialog::sendCurrentTime()
{
	const QByteArray time = QString::number(m_time.elapsed()).toUtf8();
	const QByteArray data = "TIME " + QByteArray::number(time.size()) + " " + time;
	if (m_socket->write(data) == data.size())
	 ;//isGreetingMessageSent = true;
}

void NewResultDialog::start()
{
	m_time.start();
}

void NewResultDialog::stop()
{
	m_elapsed_time = m_time.elapsed();
	const QByteArray time = QString(m_elapsed_time).toUtf8();
	const QByteArray data = "TIME " + QByteArray::number(time.size()) + " " + time;
	m_socket->write(data);
}

void NewResultDialog::refreshCompetitor(int a_round_idx)
{
	m_dialog.m_cb_competitor->clear();

	if (a_round_idx == -1)
		return;

	const int round_id = m_dialog.m_cb_round->itemData( a_round_idx ).toInt();

	//Query
	QSqlQuery query;
	query.prepare("SELECT reg.id, per.first_name, per.last_name FROM t_registrations reg "
			"JOIN t_persons per ON reg.person_id=per.id "
			"WHERE reg.id NOT IN "
			"(SELECT registration_id FROM t_results "
			"WHERE round_id=:round_id)");
	query.bindValue(":round_id", round_id);
	query.exec();

	while (query.next())
	{
		const QString name = query.value(1).toString() + " " + query.value(2).toString();
		m_dialog.m_cb_competitor->addItem(name, query.value(0).toInt());
	}
}

void NewResultDialog::refreshTime()
{
	const int msecs = m_time.elapsed();
	QTime time;
	time.addMSecs(msecs);

	m_dialog.m_lbl_time->setText( time.toString("mm:ss:zzz") );
}

void NewResultDialog::onArm()
{
	const int registration_idx = m_dialog.m_cb_competitor->currentIndex();
	if (registration_idx == -1)
		return;

	//Disable UI
	m_dialog.m_cb_competitor->setEnabled(false);
	m_dialog.m_cb_round->setEnabled(false);

	sendCompetitorInformations();
}

void NewResultDialog::onQuit()
{
	bool quit = true;
	if (m_state != FINISHED)
	{
		const QMessageBox::StandardButton res = QMessageBox::warning(
				this,
				tr("Quit"),
				tr("Are you sure you want to quit now ?"),
				QMessageBox::Ok | QMessageBox::Cancel,
				QMessageBox::Cancel);

		if (res == QMessageBox::Cancel)
			quit = false;
	}

	if (quit)
		accept();
}

void NewResultDialog::onConnected()
{
	m_state = CONNECTED;
	m_dialog.m_central_widget->setEnabled(true);
}

void NewResultDialog::onDisconnected()
{
	m_dialog.m_central_widget->setEnabled(false);
	m_state = FINISHED;

	QMessageBox::critical(
			this,
			tr("Connection error"),
			tr("Connection closed by peer!"));
}

void NewResultDialog::onHostFound()
{

}

void NewResultDialog::processSerialData()
{
	if (m_state == ARMED && 1)
		start();
}

void NewResultDialog::processSocketData()
{
	char value;
	if (m_serial_port->getChar(&value))
	{
		switch (value)
		{
		case 'A'://armed
			if (m_state == READY)
			m_state = ARMED;
			break;

		case 'F'://finished
			if (m_state == RUNNING)
				stop();
			break;

		default:
			break;
		}
	}
}
