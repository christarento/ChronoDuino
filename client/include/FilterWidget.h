/*
 * FilterWidget.h
 *
 *  Created on: 16 août 2009
 *      Author: Christo
 */

#ifndef FILTERWIDGET_H_
#define FILTERWIDGET_H_

#include <QWidget>

#include "ui_FilterWidget.h"

class FilterWidget : public QWidget
{
	Q_OBJECT
public:
	FilterWidget(QWidget* a_parent=NULL);
	virtual ~FilterWidget();

private:
	Ui::FilterWidget m_widget;
};

#endif /* FILTERWIDGET_H_ */
