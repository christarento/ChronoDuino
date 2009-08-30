/*
 * NewRoundDialog.cpp
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "ChronoContext.h"
#include "NewRoundDialog.h"

NewRoundDialog::NewRoundDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	//UI
	m_dialog.setupUi(this);

	//Connect
	connect(m_dialog.m_button_box, SIGNAL(accepted()), SLOT(onAccept()));
}

NewRoundDialog::~NewRoundDialog()
{

}

void NewRoundDialog::onAccept()
{
	const int race_id = CHRONOCONTEXT.getCurrentRace();
	if (race_id == -1)
		return;

	QString error_message = "<qt>" + tr("Please fix the following issues :") + "<ul>";
	bool warning = false;

	//Check name
	if (m_dialog.m_le_name->text().isEmpty())
	{
		warning = true;
		error_message += "<li>" + tr("<b>Name</b> field is empty") + "</li>";
	}

	if (!warning)
	{
		//Check insertion success
		QSqlQuery insert_query;
		insert_query.prepare("INSERT INTO t_rounds "
				"(race_id, name, description) "
				"VALUES(:race_id, :name, :description)");
		insert_query.bindValue(":race_id", race_id);
		insert_query.bindValue(":name", m_dialog.m_le_name->text());
		insert_query.bindValue(":description", m_dialog.m_te_description->toPlainText());

		if (!insert_query.exec())
		{
			warning = true;
			error_message += "<li>" + tr("<b>Insert Failed</b> \"%1\"").arg(insert_query.lastError().text()) + "</li>";
		}
	}


	if (warning)
	{
		error_message += "</ul></qt>";

		QMessageBox::warning(
				this,
				tr("New Round error"),
				error_message);
	}
	else
		accept();
}
