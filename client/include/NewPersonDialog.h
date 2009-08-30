/*
 * NewPersonDialog.h
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#ifndef NEWPERSONDIALOG_H_
#define NEWPERSONDIALOG_H_

#include <QDialog>

#include "ui_NewPersonDialog.h"

class NewPersonDialog : public QDialog
{
	Q_OBJECT
public:
	NewPersonDialog(QWidget* a_parent=NULL);
	virtual ~NewPersonDialog();

private:
	Ui::NewPersonDialog m_dialog;

private slots:
	void onAccept();
};

#endif /* NEWPERSONDIALOG_H_ */
