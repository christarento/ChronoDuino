/*
 * NewCategoryDialog.h
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#ifndef NEWCATEGORYDIALOG_H_
#define NEWCATEGORYDIALOG_H_

#include <QDialog>

#include "ui_NewCategoryDialog.h"

class NewCategoryDialog : public QDialog
{
	Q_OBJECT
public:
	NewCategoryDialog(QWidget* a_parent=NULL);
	virtual ~NewCategoryDialog();

private:
	Ui::NewCategoryDialog m_dialog;

private slots:
	void onAccept();
};

#endif /* NEWCATEGORYDIALOG_H_ */
