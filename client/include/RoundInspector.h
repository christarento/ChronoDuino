/*
 * RoundInspector.h
 *
 *  Created on: 15 août 2009
 *      Author: Christo
 */

#ifndef ROUNDINSPECTOR_H_
#define ROUNDINSPECTOR_H_

#include <QDockWidget>
#include <QSqlQueryModel>

#include "ui_RoundInspector.h"

class RoundInspector : public QDockWidget
{
	Q_OBJECT

public:
	RoundInspector(QWidget* a_parent=NULL);
	virtual ~RoundInspector();

public slots:
	void addRoundAction();
	void removeRoundAction();
	void editRoundAction();
	void refresh();

private:
	Ui::RoundInspector m_inspector;
	QSqlQueryModel* m_model;
};

#endif /* ROUNDINSPECTOR_H_ */
