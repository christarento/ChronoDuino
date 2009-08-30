/*
 * OpenRaceDialog.cpp
 *
 *  Created on: 1 août 2009
 *      Author: Christo
 */

#include <QSqlQuery>
#include <QSqlRecord>
#include <QTime>

#include "ChronoContext.h"
#include "OpenRaceDialog.h"

OpenRaceDialog::OpenRaceDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	//UI
	m_open_dialog.setupUi(this);

	//Connect
	connect(m_open_dialog.m_button_box, SIGNAL(accepted()), this, SLOT(onAccept()));
	connect(m_open_dialog.m_table_view, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onCompetitionSelected(const QModelIndex&)));

	//Model
	m_model = new QSqlQueryModel(this);
	m_model->setQuery("SELECT id, name, place, date FROM t_races");
	m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
	m_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	m_model->setHeaderData(2, Qt::Horizontal, tr("Place"));
	m_model->setHeaderData(3, Qt::Horizontal, tr("Date"));

	//Table View
	m_open_dialog.m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_open_dialog.m_table_view->setModel(m_model);
	m_open_dialog.m_table_view->setColumnHidden(0, true);
}

OpenRaceDialog::~OpenRaceDialog()
{

}

void OpenRaceDialog::onCompetitionSelected(const QModelIndex& a_index)
{
	const QSqlRecord record = m_model->record(a_index.row());
	const int race_id = record.value("id").toInt();

	//Update informations
	//Nb of competitors
	QSqlQuery competitors_query;
	competitors_query.prepare("SELECT COUNT(DISTINCT person_id) AS count FROM t_registrations WHERE race_id=:race_id");
	competitors_query.bindValue(":race_id", race_id);
	competitors_query.exec();
	if (competitors_query.next())
		m_open_dialog.m_la_competitors->setText(competitors_query.value(0).toString());
	else
		m_open_dialog.m_la_competitors->setText("N/A");


	//Best time
	QSqlQuery best_time_query;
	best_time_query.prepare("SELECT MIN(res.time) AS best_time "
			"FROM t_results res"
			"JOIN t_registrations reg ON res.registration_id=reg.id "
			"WHERE reg.race_id=:race_id");
	best_time_query.bindValue(":race_id", race_id);
	best_time_query.exec();
	if (best_time_query.next())
	{
		QTime time;
		time = time.addMSecs( best_time_query.value(0).toInt() );
		m_open_dialog.m_la_best_time->setText( time.toString("hh:mm:ss.zzz") );
	}
	else
		m_open_dialog.m_la_best_time->setText("N/A");
}

void OpenRaceDialog::onAccept()
{
	QModelIndex model_index = m_open_dialog.m_table_view->currentIndex();
	if (model_index.isValid())
	{
		CHRONOCONTEXT.setCurrentRace( m_model->record(model_index.row()).value(0).toInt() );
		accept();
	}
}
