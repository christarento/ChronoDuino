/*
 * NewRaceDialog.cpp
 *
 *  Created on: 4 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "ChronoContext.h"
#include "NewRaceDialog.h"

NewRaceDialog::NewRaceDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	//Ui
	m_dialog.setupUi(this);

	//Connect
	connect(m_dialog.buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

NewRaceDialog::~NewRaceDialog()
{

}

void NewRaceDialog::onAccept()
{
	bool warning = false;
	QString message = "<qt>" + tr("Are you sure you want to create a competition with :") + "<ul>";

	//Name
	if (m_dialog.m_le_name->text().isEmpty())
	{
		warning = true;
		message += "<li>" + tr("<b>Name</b> field empty") + "</li>";
	}

	//Place
	if (m_dialog.m_le_place->text().isEmpty())
	{
		warning = true;
		message += "<li>" + tr("<b>Place</b> field empty") + "</li>";
	}

	//Date
	if (m_dialog.m_de_date->date() < QDate::currentDate())
	{
		warning = true;
		message += "<li>" + tr("<b>Date</b> is before today") + "</li>";
	}

	//End
	message += "</ul></qt>";

	if (warning)
	{
		const QMessageBox::StandardButton res = QMessageBox::warning(
				NULL,
				tr("Competition creation warning"),
				message,
				QMessageBox::Ok | QMessageBox::Cancel,
				QMessageBox::Cancel);
		if (res == QMessageBox::Cancel)
			return;
	}

	//Insert into database
	QSqlQuery insert_query;
	insert_query.prepare("INSERT INTO t_races (name, place, date) "
			"VALUES (:name, :place, :date)");
	insert_query.bindValue(":name", m_dialog.m_le_name->text());
	insert_query.bindValue(":place", m_dialog.m_le_place->text());
	insert_query.bindValue(":date", m_dialog.m_de_date->date());

	if (!insert_query.exec())
	{
		QMessageBox::critical(
				this,
				tr("Race creation error"),
				insert_query.lastError().text());
		return;
	}

	QVariant last_id = insert_query.lastInsertId();
	if(last_id.isValid())
	{
		CHRONOCONTEXT.setCurrentRace( last_id.toInt() );
		accept();
	}
}
