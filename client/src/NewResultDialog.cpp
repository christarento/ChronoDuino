/*
 * NewResultDialog.cpp
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QSqlQuery>

#include "ChronoContext.h"
#include "EditPreferencesDialog.h"
#include "NewResultDialog.h"

#include "SerialPort.h"
#include "SerialThread.h"

NewResultDialog::NewResultDialog(QWidget* a_parent) :
	QDialog(a_parent),
	m_result_thread(NULL),
	m_serial_thread(NULL),
	m_state(WAITING),
	m_elapsed_time(-1)
{
	//UI
	m_dialog.setupUi(this);
	m_dialog.m_central_widget->setEnabled(false);

	//RefreshTimer
	m_refresh_timer.setInterval(200);

	//Connect
	connect(m_dialog.m_pb_quit, SIGNAL(clicked()), SLOT(onQuit()));
	connect(m_dialog.m_cb_round, SIGNAL(currentIndexChanged(int)), SLOT(refreshCompetitor(int)));
	connect(m_dialog.m_pb_arm, SIGNAL(clicked()), SLOT(onArm()));
	connect(&m_refresh_timer, SIGNAL(timeout()), SLOT(refreshTime()));

	initSerial();
}

NewResultDialog::~NewResultDialog()
{

}

void NewResultDialog::initConnection()
{
	//Socket
	m_result_thread = new NewResultThread(this);
//	connect(m_socket, SIGNAL(connected()), SLOT(onConnected()));
//	connect(m_socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
//	connect(m_socket, SIGNAL(hostFound()), SLOT(onHostFound()));
//	connect(m_socket, SIGNAL(readyRead()), SLOT(processSocketData()));

	//Settings
	const QSettings settings;
	const QString hostname = settings.value(EditPreferencesDialog::SERVER_URL).toString();
	const int port = settings.value(EditPreferencesDialog::SERVER_PORT).toInt();

	//Connection
	m_result_thread->open(hostname, port);
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
	m_serial_thread = new SerialThread(this, this);
	m_serial_thread->open(device, rate, QIODevice::ReadWrite);

	//TODO timeout
	//Send test
	char test = 'T';
	m_serial_thread->getSerialPort()->writeData(&test, 1);
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

	//Photo
	const QSettings settings;
	const QDir image_dir( settings.value(EditPreferencesDialog::IMAGES_DIR).toString() );
	const QString image_file = image_dir.filePath( query.value(2).toString() );
	const QPixmap competitor_photo(image_file);

	//Send to server
	m_result_thread->sendCompetitorInformations(
			query.value(0).toString(),
			query.value(1).toString(),
			competitor_photo,
			m_dialog.m_cb_round->currentText());
}

void NewResultDialog::sendCurrentTime()
{
	m_result_thread->sendCurrentTime( m_time.elapsed() );
}

void NewResultDialog::start()
{
	m_time.start();
	m_state = RUNNING;
	m_refresh_timer.start();
}

void NewResultDialog::stop()
{
	//Elapsed time
	m_elapsed_time = m_time.elapsed();
	m_refresh_timer.stop();

	//Send finish time to server
	m_result_thread->sendCurrentTime(m_elapsed_time);
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

	//TODO timer qui fait clignoter le armed

	//Init socket thread
	initConnection();
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

void NewResultDialog::processSerialData(const char a_value)
{
	switch (a_value)
	{
	case 'H':
		if (m_state == ARMED)
			start();
		break;

	case 'T':
		initRound();
		qDebug("Process Serial T");
		break;

	default:
		break;
	}
}

void NewResultDialog::processSocketData(const char a_value)
{
	switch (a_value)
	{
	case 'C'://connected to host
	{
		m_state = CONNECTED;
		qDebug("Process Socket connected");
		sendCompetitorInformations();
		break;
	}

	case 'A'://host armed
		m_state = ARMED;
		qDebug("Process Socket armed");
		break;

	case 'F'://finished
		if (m_state == RUNNING)
			stop();
		break;

	default:
		break;
	}
}
