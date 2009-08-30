/*
 * NewRaceDialog.h
 *
 *  Created on: 4 août 2009
 *      Author: Christo
 */

#ifndef NEWRACEDIALOG_H_
#define NEWRACEDIALOG_H_

#include <QDialog>
#include "ui_NewRaceDialog.h"

class NewRaceDialog : public QDialog
{
	Q_OBJECT
public:
	NewRaceDialog(QWidget* a_parent=NULL);
	virtual ~NewRaceDialog();

private:
	Ui::NewRaceDialog m_dialog;

private slots:
	void onAccept();
};

#endif /* NEWRACEDIALOG_H_ */
