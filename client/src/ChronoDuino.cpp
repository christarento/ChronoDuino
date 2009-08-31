/*
 * MainWindow.cpp
 *
 *  Created on: 29 juil. 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "ChronoContext.h"
#include "ChronoDuino.h"
#include "EditPreferencesDialog.h"
#include "NewRaceDialog.h"
#include "OpenRaceDialog.h"
#include "PrintDialog.h"
#include "SerialTesterDialog.h"

ChronoDuino::ChronoDuino(QWidget * a_parent)
	: QMainWindow(a_parent)
{
	m_main_window.setupUi(this);

	//UI
	m_result_inspector = new ResultInspector(this);
	setCentralWidget(m_result_inspector);

	m_competitor_inspector = new CompetitorInspector(this);
	addDockWidget(Qt::RightDockWidgetArea, m_competitor_inspector);

	m_round_inspector = new RoundInspector(this);
	addDockWidget(Qt::RightDockWidgetArea, m_round_inspector);

	//Connect
	connect(m_main_window.m_action_new, SIGNAL(triggered()), SLOT(newRaceAction()));
	connect(m_main_window.m_action_open, SIGNAL(triggered()), SLOT(openRaceAction()));
	connect(m_main_window.m_action_preferences, SIGNAL(triggered()), SLOT(editPreferencesAction()));
	connect(m_main_window.m_action_about, SIGNAL(triggered()), SLOT(aboutAction()));
	connect(m_main_window.m_action_print, SIGNAL(triggered()), SLOT(printAction()));
	connect(m_main_window.m_action_test, SIGNAL(triggered()), SLOT(testAction()));

	initDbConnection();
}

ChronoDuino::~ChronoDuino()
{
	ChronoContext::release();
}

void ChronoDuino::showMessage(const QString& a_message)
{
	statusBar()->showMessage(a_message, 5000);
}

void ChronoDuino::initRace()
{
	const int race_id = CHRONOCONTEXT.getCurrentRace();

	m_competitor_inspector->refresh();
	m_round_inspector->refresh();
	m_result_inspector->refresh();

	QSqlQuery name_query;
	name_query.prepare("SELECT name FROM t_races "
			"WHERE id=:id");
	name_query.bindValue(":id", race_id);
	name_query.exec();

	if (name_query.next())
		showMessage(tr("Competition %1 opened").arg(name_query.value(0).toString()));
}


///////////////////
// SLOTS
void ChronoDuino::initDbConnection()
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid())
		return;

	const QSettings settings;
	const QString file_name = settings.value(EditPreferencesDialog::DB_SQLITE_FILE).toString();
	if (!file_name.isEmpty())
	{
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName( file_name );

		if (db.open())
		{
			m_main_window.m_status_bar->showMessage(tr("Connection to SQlite database %1 OK.").arg(file_name));
			return;
		}
	}

	QMessageBox::critical(
			NULL,
			tr("Database Connection"),
			tr("<qt>Error while trying to open SQlite database <b>%1<b>.<br/>Please check your connection settings.</qt>").arg(file_name));
}

void ChronoDuino::newRaceAction()
{
	NewRaceDialog new_race(this);

	if (new_race.exec() == QDialog::Accepted)
		initRace();
}

void ChronoDuino::openRaceAction()
{
	OpenRaceDialog open_race(this);

	if (open_race.exec() == QDialog::Accepted)
		initRace();
}

void ChronoDuino::editPreferencesAction()
{
	EditPreferencesDialog edit_dialog(this);

	if (edit_dialog.exec() == QDialog::Accepted)
		m_main_window.m_status_bar->showMessage(tr("Preferences saved."), 2000);
}

void ChronoDuino::printAction()
{
	PrintDialog print_dialog(this);
	print_dialog.exec();
}

void ChronoDuino::testAction()
{
	//Settings
	const QSettings settings;
	const QString device = settings.value(EditPreferencesDialog::SERIAL_PORT).toString();
	const int rate = settings.value(EditPreferencesDialog::SERIAL_RATE).toInt();

	SerialTesterDialog test_dialog(device, rate, this);
	test_dialog.exec();
}

void ChronoDuino::aboutAction()
{
	QMessageBox::about(
			this,
			tr("About ChronoDuino"),
			tr("The <b>Menu</b> example shows how to create menu-bar menus and context menus."));
}
