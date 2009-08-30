/*
 * NewPersonDialog.cpp
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "NewPersonDialog.h"

NewPersonDialog::NewPersonDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	m_dialog.setupUi(this);

	//Connect
	connect(m_dialog.m_button_box, SIGNAL(accepted()), SLOT(onAccept()));
}

NewPersonDialog::~NewPersonDialog()
{

}

void NewPersonDialog::onAccept()
{
	QString error_message = "<qt>" + tr("Please fix the following issues :") + "<ul>";
	bool warning = false;

	//Check first name
	if (m_dialog.m_le_first_name->text().isEmpty())
	{
		warning = true;
		error_message += "<li>" + tr("<b>First Name</b> field is empty") + "</li>";
	}

	//Check last name
	if (m_dialog.m_le_last_name->text().isEmpty())
	{
		warning = true;
		error_message += "<li>" + tr("<b>Last Name</b> field is empty") + "</li>";
	}


	if (!warning)
	{
		//Check insertion success
		QSqlQuery insert_query;
		insert_query.prepare("INSERT INTO t_persons "
				"(first_name, last_name, representative, birthday, address, phone, email) "
				"VALUES(:first_name, :last_name, :representative, :birthday, :address, :phone, :email)");
		insert_query.bindValue(":first_name", m_dialog.m_le_first_name->text());
		insert_query.bindValue(":last_name", m_dialog.m_le_last_name->text());
		insert_query.bindValue(":representative", m_dialog.m_le_representative->text());
		insert_query.bindValue(":birthday", m_dialog.m_de_birthday->date());
		insert_query.bindValue(":address", m_dialog.m_te_address->toPlainText());
		insert_query.bindValue(":phone", m_dialog.m_le_phone->text());
		insert_query.bindValue(":email", m_dialog.m_le_email->text());

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
				tr("New person error"),
				error_message);
	}
	else
		accept();
}
