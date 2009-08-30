/*
 * RoundInspector.cpp
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

#include "ChronoContext.h"
#include "NewRoundDialog.h"
#include "RoundInspector.h"

RoundInspector::RoundInspector(QWidget* a_parent) :
	QDockWidget(a_parent)
{
	//UI
	m_inspector.setupUi(this);

	//Model view
	m_model = new QSqlQueryModel(this);

	//Table view
	m_inspector.m_table_view->setModel(m_model);
	m_inspector.m_table_view->verticalHeader()->setVisible(false);

	//Connect
	connect(m_inspector.m_action_add, SIGNAL(triggered()), SLOT(addRoundAction()));
	connect(m_inspector.m_action_remove, SIGNAL(triggered()), SLOT(removeRoundAction()));
	connect(m_inspector.m_action_edit, SIGNAL(triggered()), SLOT(editRoundAction()));

	refresh();
}

RoundInspector::~RoundInspector()
{

}

void RoundInspector::addRoundAction()
{
	NewRoundDialog new_dialog;
	if (new_dialog.exec())
		refresh();
}

void RoundInspector::removeRoundAction()
{
	const QModelIndex index = m_inspector.m_table_view->currentIndex();
	if (!index.isValid())
		return;

	const QSqlRecord record = m_model->record(index.row());
	const QString name = record.value("name").toString();

	const QMessageBox::StandardButton res = QMessageBox::question(this,
			tr("Remove Round"),
			tr("Are you sure you want to remove <b>%1</b>").arg(name),
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok);

	if (res == QMessageBox::Ok)
	{
		QSqlQuery remove_query;
		remove_query.prepare("DELETE FROM t_rounds WHERE id=:id");
		remove_query.bindValue(":id", record.value("id").toInt());
		remove_query.exec();

		refresh();
	}
}

void RoundInspector::editRoundAction()
{

}

void RoundInspector::refresh()
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
	query.prepare("SELECT id, name, description FROM t_rounds "
			"WHERE race_id=:race_id");
	query.bindValue(":race_id", race_id);
	query.exec();

	//Model
	m_model->setQuery(query);
	m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
	m_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	m_model->setHeaderData(2, Qt::Horizontal, tr("Description"));

	//View
	m_inspector.m_table_view->setColumnHidden(0, true);
}
