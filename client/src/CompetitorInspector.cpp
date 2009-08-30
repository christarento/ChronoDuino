/*
 * CompetitorInspector.cpp
 *
 *  Created on: 2 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QToolBar>

#include "ChronoContext.h"
#include "CompetitorInspector.h"
#include "NewCompetitorDialog.h"

CompetitorInspector::CompetitorInspector(QWidget* a_parent) :
	QDockWidget(a_parent)
{
	//UI
	m_inspector.setupUi(this);

	//Model
	m_model = new QSqlQueryModel(this);

	//Table View
	m_inspector.m_table_view->setModel(m_model);
	m_inspector.m_table_view->verticalHeader()->setVisible(false);

	//Connect
	connect(m_inspector.m_action_add, SIGNAL(triggered()), SLOT(addCompetitorAction()));
	connect(m_inspector.m_action_remove, SIGNAL(triggered()), SLOT(removeCompetitorAction()));
	connect(m_inspector.m_action_edit, SIGNAL(triggered()), SLOT(editCompetitorAction()));
	connect(m_inspector.m_table_view, SIGNAL(activated(const QModelIndex&)), SLOT(onModelSelected(const QModelIndex&)));

	refresh();
}

CompetitorInspector::~CompetitorInspector()
{

}

void CompetitorInspector::refresh()
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
	query.prepare("SELECT reg.id AS reg_id, per.id AS per_id, per.first_name AS per_first_name, per.last_name AS per_last_name, cat.name AS cat_name "
			"FROM t_registrations reg "
			"JOIN t_persons per ON reg.person_id=per.id "
			"JOIN t_categories cat ON reg.category_id=cat.id "
			"WHERE cat.race_id=:race_id");
	query.bindValue(":race_id", race_id);
	query.exec();

	//Model
	m_model->setQuery(query);
	m_model->setHeaderData(0, Qt::Horizontal, tr("Reg ID"));
	m_model->setHeaderData(1, Qt::Horizontal, tr("Pers ID"));
	m_model->setHeaderData(2, Qt::Horizontal, tr("First Name"));
	m_model->setHeaderData(3, Qt::Horizontal, tr("Last Name"));
	m_model->setHeaderData(4, Qt::Horizontal, tr("Category"));

	//View
	m_inspector.m_table_view->setColumnHidden(0, true);
	m_inspector.m_table_view->setColumnHidden(1, true);
}

void CompetitorInspector::addCompetitorAction()
{
	NewCompetitorDialog new_dialog(this);

	if (new_dialog.exec() == QDialog::Accepted)
	{
		refresh();
		emit informationMessage("New Competitor added");
	}
}

void CompetitorInspector::removeCompetitorAction()
{
	const QModelIndex index = m_inspector.m_table_view->currentIndex();
	if (!index.isValid())
		return;

	const QSqlRecord record = m_model->record(index.row());
	const QString name = record.value("per_first_name").toString() + " " + record.value("per_last_name").toString();

	const QMessageBox::StandardButton res = QMessageBox::question(this,
			tr("Remove Competitor"),
			tr("Are you sure you want to remove <b>%1</b>").arg(name),
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok);

	if (res == QMessageBox::Ok)
	{
		QSqlQuery remove_query;
		remove_query.prepare("DELETE FROM t_registrations WHERE id=:id");
		remove_query.bindValue(":id", record.value("reg_id").toInt());
		remove_query.exec();

		refresh();
	}
}

void CompetitorInspector::editCompetitorAction()
{
	//TODO
}

void CompetitorInspector::onModelSelected(const QModelIndex& a_index)
{
	//TODO, Tooltip
}
