/*
 * NewResultDialog.cpp
 *
 *  Created on: 15 ao�t 2009
 *      Author: Christo
 */

#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>

#include "ChronoContext.h"
#include "EditPreferencesDialog.h"
#include "NewResultDialog.h"

#include "SerialPort.h"
#include "SerialThread.h"

NewResultDialog::NewResultDialog(QWidget* a_parent) :
	QDialog(a_parent),
	m_result_thread(NULL),
	m_serial_thread(NULL),
	m_state(WAITING)
{
	//UI
	m_dialog.setupUi(this);
	m_dialog.m_central_widget->setEnabled(false);

	//Sound
	const QDir sound_dir(qApp->applicationDirPath() + "/sounds");
	m_armed_sound = new QSound(sound_dir.filePath("armed.wav"), this);
	m_sensor_sound = new QSound(sound_dir.filePath("sensor.wav"), this);
	m_finished_sound = new QSound(sound_dir.filePath("finished.wav"), this);

	//Serial
	m_serial_thread = new SerialThread(this, this);
	connect(m_serial_thread, SIGNAL(connected()), SLOT(sendCompetitorInformations()));
	connect(this, SIGNAL(started()), SLOT(start()));

	//Thread
	m_result_thread = new NewResultThread(this);
	connect(m_result_thread, SIGNAL(connected()), SLOT(refreshRound()));
	connect(m_result_thread, SIGNAL(armed()), SLOT(stateArmed()));
	connect(m_result_thread, SIGNAL(currentTime(const int&)), SLOT(refreshTime(const int&)));
	connect(m_result_thread, SIGNAL(finished(const int&)), SLOT(stop(const int&)));
	connect(m_result_thread, SIGNAL(error(const QString&)), SLOT(socketError(const QString&)));

	//Connect
	connect(m_dialog.m_pb_quit, SIGNAL(clicked()), SLOT(onQuit()));
	connect(m_dialog.m_cb_round, SIGNAL(currentIndexChanged(int)), SLOT(refreshCompetitor(int)));
	connect(m_dialog.m_pb_arm, SIGNAL(clicked()), SLOT(onArm()));

	initConnection();
}

NewResultDialog::~NewResultDialog()
{

}

void NewResultDialog::initConnection()
{
	qDebug("NewResultDialog::initConnection()");

	//Settings
	const QSettings settings;
	const QString hostname = settings.value(EditPreferencesDialog::SERVER_URL).toString();
	const int port = settings.value(EditPreferencesDialog::SERVER_PORT).toInt();

	//Connection
	m_result_thread->open(hostname, port);
}

void NewResultDialog::refreshRound()
{
	m_dialog.m_central_widget->setEnabled(true);
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
	qDebug("NewResultDialog::initSerial()");

	//Settings
	const QSettings settings;
	const QString device = settings.value(EditPreferencesDialog::SERIAL_PORT).toString();
	const int rate = settings.value(EditPreferencesDialog::SERIAL_RATE).toInt();

	//Serial port
	m_serial_thread->open(device, rate, QIODevice::ReadWrite);
}

void NewResultDialog::sendCompetitorInformations()
{
	qDebug("NewResultDialog::sendCompetitorInformations()");

	const int registration_idx = m_dialog.m_cb_competitor->currentIndex();
	const int registration_id = m_dialog.m_cb_competitor->itemData(registration_idx).toInt();
	QSqlQuery query;
	query.prepare("SELECT per.first_name, per.last_name, per.photo FROM t_registrations reg "
			"JOIN t_persons per ON reg.person_id=per.id "
			"WHERE reg.id=:registration_id");
	query.bindValue(":registration_id", registration_id);

	if (!query.exec())
		QMessageBox::critical(
				this,
				tr("Internal error"),
				tr("Database error : %1").arg(query.lastError().text()));

	query.next();

	//Photo
	const QSettings settings;
	const QDir image_dir( qApp->applicationDirPath() );
	const QString image_file = image_dir.filePath( query.value(2).toString() );
	const QPixmap competitor_photo(image_file);

	//Send to server
	m_result_thread->sendCompetitorInformations(
			query.value(0).toString(),
			query.value(1).toString(),
			competitor_photo,
			m_dialog.m_cb_round->currentText());
}

void NewResultDialog::stateArmed()
{
	qDebug("NewResultDialog::stateArmed()");
	m_dialog.m_lbl_status->setText(tr("Armed ..."));

	m_state = ARMED;
	m_armed_sound->play();
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

void NewResultDialog::refreshTime(const int& a_time)
{
	QTime time(0, 0);
	time = time.addMSecs(a_time);

	//UI
	m_dialog.m_lbl_time->setText( time.toString("mm:ss:zzz") );
}

void NewResultDialog::start()
{
	m_state = RUNNING;
	m_dialog.m_lbl_status->setText(tr("Running ..."));

	m_sensor_sound->play();
}

void NewResultDialog::stop(const int& a_time)
{
	//UI
	refreshTime(a_time);

	m_state = FINISHED;
	m_dialog.m_lbl_status->setText(tr("Finished ..."));

	m_finished_sound->play();


	//Insert into database
	const int competitor_idx = m_dialog.m_cb_competitor->currentIndex();
	const int round_idx = m_dialog.m_cb_round->currentIndex();

	QSqlQuery insert_query;
	insert_query.prepare("INSERT into t_results (registration_id, round_id, time) "
			"VALUES (:reg_id, :rnd_id, :time)");
	insert_query.bindValue(":reg_id", m_dialog.m_cb_competitor->itemData(competitor_idx));
	insert_query.bindValue(":rnd_id", m_dialog.m_cb_round->itemData(round_idx));
	insert_query.bindValue(":time", a_time);

	if (!insert_query.exec())
		QMessageBox::critical(
				this,
				tr("Insert error"),
				tr("Error while inserting new result : %1").arg(insert_query.lastError().text()));
}

void NewResultDialog::onArm()
{
	m_dialog.m_pb_arm->setEnabled(false);

	const int competitor_idx = m_dialog.m_cb_competitor->currentIndex();
	if (competitor_idx == -1)
		return;

	const int round_idx = m_dialog.m_cb_round->currentIndex();
	if (round_idx == -1)
		return;

	//Disable UI
	m_dialog.m_cb_competitor->setEnabled(false);
	m_dialog.m_cb_round->setEnabled(false);

	initSerial();
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

void NewResultDialog::socketError(const QString& a_message)
{
	m_dialog.m_central_widget->setEnabled(false);
	m_state = FINISHED;

	QMessageBox::critical(
			this,
			tr("Connection error"),
			a_message);
}

void NewResultDialog::processSerialData(const char a_value)
{
	switch (a_value)
	{
	case 'H':
		if (m_state == ARMED)
		{
			m_result_thread->startChrono();
			emit started();
		}
		break;

	default:
		break;
	}
}
