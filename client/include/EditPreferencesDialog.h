/*
 * EditPreferencesDialog.h
 *
 *  Created on: 30 juil. 2009
 *      Author: Christo
 */

#ifndef EDITPREFERENCESDIALOG_H_
#define EDITPREFERENCESDIALOG_H_

#include <QButtonGroup>
#include <QDialog>
#include "ui_EditPreferencesDialog.h"

class EditPreferencesDialog : public QDialog
{
	Q_OBJECT
public:
	EditPreferencesDialog(QWidget * a_parent=NULL);
	virtual ~EditPreferencesDialog();

	static const QString DB_CONNECTION_DRIVER;
	static const QString DB_SQLITE_FILE;
	static const QString DB_SERVER_NAME;
	static const QString DB_SERVER_URL;
	static const QString DB_SERVER_PORT;
	static const QString SERVER_URL;
	static const QString SERVER_PORT;
	static const QString SERIAL_PORT;
	static const QString SERIAL_RATE;
	static const QString IMAGES_DIR;

private:
	Ui::EditPreferencesDialog m_dialog;
	QButtonGroup * m_button_group;

private slots:
	void onAccept();
	void onSQliteFilePathClicked();
	void onSQLDriverChange(QAbstractButton* a_button);
};

#endif /* EDITPREFERENCESDIALOG_H_ */
