/*
 * ResultInspector.cpp
 *
 *  Created on: 16 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTime>

#include "ChronoContext.h"
#include "NewResultDialog.h"
#include "ResultInspector.h"

ResultInspector::ResultInspector(QWidget* a_parent) :
	QWidget(a_parent)
{
	//UI
	m_inspector.setupUi(this);
	setEnabled(false);

//	m_filter_widget = new FilterWidget(this);
//	m_inspector.m_tool_bar->addWidget(m_filter_widget);

	m_inspector.m_tool_bar->addAction(m_inspector.m_action_add);
	m_inspector.m_tool_bar->addAction(m_inspector.m_action_remove);


	//Model view
	m_model = new QSqlQueryModel(this);

	//Table view
	m_inspector.m_table_view->setModel(m_model);
	m_inspector.m_table_view->verticalHeader()->setVisible(false);

	//Connect
//	connect(m_filter_widget, SIGNAL(filterChanged(QString)), this, SLOT(setFilter(QString)));
	connect(m_inspector.m_action_add, SIGNAL(triggered()), SLOT(addResultAction()));
	connect(m_inspector.m_action_remove, SIGNAL(triggered()), SLOT(removeResultAction()));
}

ResultInspector::~ResultInspector()
{

}

void ResultInspector::addResultAction()
{
	NewResultDialog new_dialog(this);
	if (new_dialog.exec())
		refresh();
}

void ResultInspector::removeResultAction()
{
	QModelIndex index = m_inspector.m_table_view->currentIndex();
	if (!index.isValid())
		return;

	const QSqlRecord record = m_model->record(index.row());
	QTime time;
	time = time.addMSecs(record.value("res_time").toInt());
	const QString name = record.value("per_first_name").toString() + " " + record.value("per_last_name").toString();

	const QMessageBox::StandardButton res = QMessageBox::question(
			this,
			tr("Remove Result"),
			tr("Are you sure you want to remove time <b>%1</b> of <b>%2</b>").arg(time.toString("hh:mm:ss.zzz")).arg(name),
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok);

	if (res == QMessageBox::Ok)
	{
		QSqlQuery remove_query;
		remove_query.prepare("DELETE FROM t_results "
				"WHERE id=:id");
		remove_query.bindValue(":id", record.value("res_id").toInt());
		remove_query.exec();

		refresh();
	}
}

void ResultInspector::refresh()
{
	m_model->clear();

	//Race id
	const int race_id = CHRONOCONTEXT.getCurrentRace();
	if (race_id == -1)
	{
		setEnabled(false);
		return;
	}
	else
		setEnabled(true);

	//Query
	QSqlQuery query;
	query.prepare("SELECT res.id AS res_id, rnd.name AS rnd_name, res.time AS res_time, per.first_name AS per_first_name, per.last_name AS per_last_name "
				"FROM t_results res "
				"JOIN t_registrations reg ON res.registration_id=reg.id "
				"JOIN t_persons per ON reg.person_id=per.id "
				"JOIN t_rounds rnd ON res.round_id=rnd.id "
				"WHERE rnd.race_id=:race_id "
				"ORDER BY rnd_name, res_time");
	query.bindValue(":race_id", race_id);
	query.exec();

	//Model
	m_model->setQuery(query);
	m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
	m_model->setHeaderData(1, Qt::Horizontal, tr("Round"));
	m_model->setHeaderData(2, Qt::Horizontal, tr("Time"));
	m_model->setHeaderData(3, Qt::Horizontal, tr("First Name"));
	m_model->setHeaderData(4, Qt::Horizontal, tr("Last Name"));

	//View
	m_inspector.m_table_view->setColumnHidden(0, true);
}

void ResultInspector::setFilter(const QString& a_filter)
{

}
