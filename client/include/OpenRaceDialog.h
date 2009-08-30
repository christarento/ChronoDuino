/*
 * OpenRaceDialog.h
 *
 *  Created on: 1 août 2009
 *      Author: Christo
 */

#ifndef OPENRACEDIALOG_H_
#define OPENRACEDIALOG_H_

#include <QDialog>
#include <QSqlQueryModel>

#include "ui_OpenRaceDialog.h"

class OpenRaceDialog : public QDialog
{
	Q_OBJECT
public:
	OpenRaceDialog(QWidget* a_parent=NULL);
	virtual ~OpenRaceDialog();

private:
	Ui::OpenRaceDialog m_open_dialog;
	QSqlQueryModel* m_model;

private slots:
	void onAccept();
	void onCompetitionSelected(const QModelIndex& a_index);
};

#endif /* OPENRACEDIALOG_H_ */
