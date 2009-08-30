/*
 * NewRoundDialog.h
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#ifndef NEWROUNDDIALOG_H_
#define NEWROUNDDIALOG_H_

#include <QDialog>

#include "ui_NewRoundDialog.h"

class NewRoundDialog : public QDialog
{
	Q_OBJECT
public:
	NewRoundDialog(QWidget* a_parent=NULL);
	virtual ~NewRoundDialog();

private:
	Ui::NewRoundDialog m_dialog;

private slots:
	void onAccept();
};

#endif /* NEWROUNDDIALOG_H_ */
