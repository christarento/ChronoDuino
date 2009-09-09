/*
 * EditPreferencesDialog.cpp
 *
 *  Created on: 30 juil. 2009
 *      Author: Christo
 */

#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QSettings>
#include <QSpinBox>

#include "EditPreferencesDialog.h"
#include "SerialPort.h"

const QString EditPreferencesDialog::DB_CONNECTION_DRIVER = "db_driver";
const QString EditPreferencesDialog::DB_SQLITE_FILE = "db/sqlite/file";
const QString EditPreferencesDialog::DB_SERVER_NAME = "db/server/name";
const QString EditPreferencesDialog::DB_SERVER_URL = "db/server/url";
const QString EditPreferencesDialog::DB_SERVER_PORT = "db/server/port";
const QString EditPreferencesDialog::SERVER_URL = "server/url";
const QString EditPreferencesDialog::SERVER_PORT = "server/port";
const QString EditPreferencesDialog::SERIAL_PORT = "serial/port";
const QString EditPreferencesDialog::SERIAL_RATE = "serial/rate";
const QString EditPreferencesDialog::IMAGES_DIR = "images_directory";

EditPreferencesDialog::EditPreferencesDialog(QWidget * a_parent):
	QDialog(a_parent)
{
	//Create UI
	m_dialog.setupUi(this);

	//Button group : exclusive radio buttons
	m_button_group = new QButtonGroup(this);
	m_button_group->addButton(m_dialog.m_rb_sqlite);
	m_button_group->addButton(m_dialog.m_rb_mysql);
	m_button_group->addButton(m_dialog.m_rb_pgsql);
	connect(m_button_group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onSQLDriverChange(QAbstractButton*)));

	//Read from QSettings
	const QSettings settings;

	//Driver
	const QString& db_driver = settings.value(DB_CONNECTION_DRIVER, "QSQLITE").toString();
	if (db_driver == "QSQLITE")
		m_dialog.m_rb_sqlite->click();
	else if (db_driver == "QMYSQL")
		m_dialog.m_rb_mysql->click();
	else if (db_driver == "QPSQL")
		m_dialog.m_rb_pgsql->click();

	//SQlite
	m_dialog.m_le_sqlite_filepath->setText( settings.value(DB_SQLITE_FILE).toString() );

	//Database server
	m_dialog.m_le_db_name->setText( settings.value(DB_SERVER_NAME).toString() );
	m_dialog.m_le_db_url->setText( settings.value(DB_SERVER_URL, "localhost").toString() );
	m_dialog.m_sb_db_port->setValue( settings.value(DB_SERVER_PORT, 3306).toInt() );

	//Server
	m_dialog.m_le_server_url->setText( settings.value(SERVER_URL, "localhost").toString() );
	m_dialog.m_sb_server_port->setValue( settings.value(SERVER_PORT, 8000).toInt() );

	//Serial com
	QStringList serial_list = SerialPort::listSerialPort();
	m_dialog.m_cb_serial_port->addItems(serial_list);
	const int port_idx = m_dialog.m_cb_serial_port->findText( settings.value(SERIAL_PORT).toString() );
	if (port_idx != -1)
		m_dialog.m_cb_serial_port->setCurrentIndex(port_idx);

	const int rate_idx = m_dialog.m_cb_serial_rate->findText( settings.value(SERIAL_RATE, 9600).toString() );
	if (rate_idx != -1)
		m_dialog.m_cb_serial_rate->setCurrentIndex(rate_idx);

	//Connect
	connect(m_dialog.m_button_box, SIGNAL(accepted()), this, SLOT(onAccept()));
	connect(m_dialog.m_pb_sqlite_path, SIGNAL(clicked(bool)), this, SLOT(onSQliteFilePathClicked()));
}

EditPreferencesDialog::~EditPreferencesDialog()
{

}

void EditPreferencesDialog::onAccept()
{
	//Write to QSettings
	QSettings settings;

	//Driver
	if (m_button_group->checkedButton() == m_dialog.m_rb_mysql)
		settings.setValue(DB_CONNECTION_DRIVER, "QMYSQL");
	else if (m_button_group->checkedButton() == m_dialog.m_rb_pgsql)
		settings.setValue(DB_CONNECTION_DRIVER, "QPSQL");
	else
		settings.setValue(DB_CONNECTION_DRIVER, "QSQLITE");

	//SQlite
	settings.setValue(DB_SQLITE_FILE, m_dialog.m_le_sqlite_filepath->text());

	//Database server
	settings.setValue(DB_SERVER_NAME, m_dialog.m_le_db_name->text());
	settings.setValue(DB_SERVER_URL, m_dialog.m_le_db_url->text());
	settings.setValue(DB_SERVER_PORT, m_dialog.m_sb_db_port->value());

	//Server
	settings.setValue(SERVER_URL, m_dialog.m_le_server_url->text());
	settings.setValue(SERVER_PORT, m_dialog.m_sb_server_port->value());

	//Serial com
	if (m_dialog.m_cb_serial_port->count())
		settings.setValue(SERIAL_PORT, m_dialog.m_cb_serial_port->currentText());
	settings.setValue(SERIAL_RATE, m_dialog.m_cb_serial_rate->currentText());

	accept();
}

void EditPreferencesDialog::onSQliteFilePathClicked()
{
	QString path = m_dialog.m_le_sqlite_filepath->text();
	if (path.isEmpty())
		path = QDir::home().absolutePath();

	QFileDialog file_dialog(NULL,
			tr("SQLite file path"),
			path,
			tr("SQlite file (*.sqlite)"));
	file_dialog.setFileMode(QFileDialog::AnyFile);
	file_dialog.setDefaultSuffix("sqlite");

	if (file_dialog.exec() == QFileDialog::Accepted)
	{
		QStringList fileNames = file_dialog.selectedFiles();
		if (!fileNames.isEmpty())
		{
			path = fileNames.first();
			m_dialog.m_le_sqlite_filepath->setText( path );
		}
	}
}

void EditPreferencesDialog::onSQLDriverChange(QAbstractButton* a_button)
{
	if (a_button == m_dialog.m_rb_sqlite)
		m_dialog.m_gb_db_server->setEnabled(false);
	else
		m_dialog.m_gb_sqlite->setEnabled(true);
}
