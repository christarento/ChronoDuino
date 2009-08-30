/*
 * EditPreferencesDialog.h
 *
 *  Created on: 9 août 2009
 *      Author: Christo
 */

#ifndef EDITPREFERENCESDIALOG_H_
#define EDITPREFERENCESDIALOG_H_

#include <QDialog>

#include "ui_EditPreferencesDialog.h"

class EditPreferencesDialog : public QDialog
{
	Q_OBJECT
public:
	static const QString BIND_PORT;
	static const QString SERIAL_PORT;
	static const QString SERIAL_RATE;

	EditPreferencesDialog(QWidget* a_parent=NULL);
	virtual ~EditPreferencesDialog();

private:
	Ui::EditPreferencesDialog m_dialog;

private slots:
	void onAccept();
};

#endif /* EDITPREFERENCESDIALOG_H_ */
