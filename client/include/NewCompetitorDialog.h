/*
 * NewCompetitorDialog.h
 *
 *  Created on: 20 août 2009
 *      Author: Christo
 */

#ifndef NEWCOMPETITORDIALOG_H_
#define NEWCOMPETITORDIALOG_H_

#include <QDialog>

#include "ui_NewCompetitorDialog.h"

class NewCompetitorDialog : public QDialog
{
	Q_OBJECT
public:
	NewCompetitorDialog(QWidget* a_parent=NULL);
	virtual ~NewCompetitorDialog();

private:
	void refresh();

	Ui::NewCompetitorDialog m_dialog;

private slots:
	void onAccept();
	void addPerson();
	void addCategory();
};

#endif /* NEWCOMPETITORDIALOG_H_ */
