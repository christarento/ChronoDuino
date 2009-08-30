/*
 * ResultInspector.h
 *
 *  Created on: 16 août 2009
 *      Author: Christo
 */

#ifndef RESULTINSPECTOR_H_
#define RESULTINSPECTOR_H_

#include <QSqlQueryModel>
#include <QWidget>

#include "FilterWidget.h"
#include "ui_ResultInspector.h"

class ResultInspector : public QWidget
{
	Q_OBJECT
public:
	ResultInspector(QWidget* a_parent=NULL);
	virtual ~ResultInspector();

public slots:
	void setFilter(const QString& a_filter);
	void refresh();

private:
	Ui::ResultInspector m_inspector;
	FilterWidget* m_filter_widget;

	QSqlQueryModel* m_model;

private slots:
	void addResultAction();
	void removeResultAction();
};

#endif /* RESULTINSPECTOR_H_ */
