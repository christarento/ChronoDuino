/*
 * NewCompetitorDialog.cpp
 *
 *  Created on: 20 août 2009
 *      Author: Christo
 */

#include <QComboBox>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "ChronoContext.h"
#include "NewCategoryDialog.h"
#include "NewCompetitorDialog.h"
#include "NewPersonDialog.h"

NewCompetitorDialog::NewCompetitorDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	//UI
	m_dialog.setupUi(this);

	//Connect()
	connect(m_dialog.m_button_box, SIGNAL(accepted()), SLOT(onAccept()));
	connect(m_dialog.m_pb_add_person, SIGNAL(clicked()), SLOT(addPerson()));
	connect(m_dialog.m_pb_add_category, SIGNAL(clicked()), SLOT(addCategory()));

	refresh();
}

NewCompetitorDialog::~NewCompetitorDialog()
{

}

void NewCompetitorDialog::refresh()
{
	//Clear UI
	m_dialog.m_cb_person->clear();
	m_dialog.m_cb_category->clear();

	//Race
	const int race_id = CHRONOCONTEXT.getCurrentRace();


	//Person
	QSqlQuery person_query;
	person_query.prepare("SELECT id, first_name, last_name FROM t_persons "
			"WHERE id NOT IN "
				"(SELECT person_id FROM t_registrations re "
				"JOIN t_categories ca ON re.category_id=ca.id "
				"WHERE ca.race_id=:race_id)");
	person_query.bindValue(":race_id", race_id);
	person_query.exec();

	while (person_query.next())
		m_dialog.m_cb_person->addItem(
				person_query.value(1).toString() + " " + person_query.value(2).toString(),
				person_query.value(0));


	//Category
	QSqlQuery category_query;
	category_query.prepare("SELECT id, name FROM t_categories "
			"WHERE race_id=:race_id");
	category_query.bindValue(":race_id", race_id);
	category_query.exec();

	while (category_query.next())
		m_dialog.m_cb_category->addItem(
				category_query.value(1).toString(),
				category_query.value(0));


	//Number
	QSqlQuery number_query;
	number_query.prepare("SELECT DISTINCT number FROM t_registrations re "
			"JOIN t_categories ca ON re.category_id=ca.id "
			"WHERE ca.race_id=:race_id");
	number_query.bindValue(":race_id", race_id);
	number_query.exec();

	QSet<int> available_numbers;
	available_numbers.reserve(99);
	for (int i=1; i<100; ++i)
		available_numbers.insert(i);

	while (number_query.next())
		available_numbers.remove(number_query.value(0).toInt());

	QSet<int>::const_iterator it = available_numbers.constBegin();
	QSet<int>::const_iterator it_end = available_numbers.constEnd();
	for(; it!=it_end; ++it)
		m_dialog.m_cb_number->addItem(
				QString::number(*it),
				*it);
}

void NewCompetitorDialog::onAccept()
{
	//Check combo box index
	const int person_idx = m_dialog.m_cb_person->currentIndex();
	if (person_idx == -1)
		return;

	const int category_idx = m_dialog.m_cb_category->currentIndex();
	if (category_idx == -1)
		return;

	const int number_idx = m_dialog.m_cb_number->currentIndex();
	if (number_idx == -1)
		return;


	//Get combo box value
	const int person_id = m_dialog.m_cb_person->itemData(person_idx).toInt();
	const int category_id = m_dialog.m_cb_category->itemData(category_idx).toInt();
	const int number = m_dialog.m_cb_number->itemData(number_idx).toInt();

	QSqlQuery insert_query;
	insert_query.prepare("INSERT INTO t_registrations (person_id, category_id, number) "
			"VALUES(:person_id, :category_id, :number)");
	insert_query.bindValue(":person_id", person_id);
	insert_query.bindValue(":category_id", category_id);
	insert_query.bindValue(":number", number);

	if (!insert_query.exec())
	{
		QMessageBox::critical(
				this,
				tr("New competitor"),
				tr("<b>Insertion Failed</b> \"%1\"").arg(insert_query.lastError().text()));
	}
	else
		accept();
}

void NewCompetitorDialog::addPerson()
{
	NewPersonDialog new_dialog(this);
	if (new_dialog.exec())
		refresh();
}

void NewCompetitorDialog::addCategory()
{
	NewCategoryDialog new_dialog(this);
	if (new_dialog.exec())
		refresh();
}
