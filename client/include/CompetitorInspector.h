/*
 * CompetitorInspector.h
 *
 *  Created on: 2 août 2009
 *      Author: Christo
 */

#ifndef COMPETITORINSPECTOR_H_
#define COMPETITORINSPECTOR_H_

#include <QDockWidget>
#include <QSqlQueryModel>

#include "ui_CompetitorInspector.h"

class CompetitorInspector : public QDockWidget
{
	Q_OBJECT
public:
	CompetitorInspector(QWidget* a_parent=NULL);
	virtual ~CompetitorInspector();

	void refresh();

signals:
	void informationMessage(const QString&);

private:
	Ui::CompetitorInspector m_inspector;

	QSqlQueryModel* m_model;

private slots:
	void addCompetitorAction();
	void removeCompetitorAction();
	void editCompetitorAction();

	void onModelSelected(const QModelIndex& a_index);
};

#endif /* COMPETITORINSPECTOR_H_ */
