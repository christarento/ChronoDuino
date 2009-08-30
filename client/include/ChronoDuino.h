/*
 * MainWindow.h
 *
 *  Created on: 29 juil. 2009
 *      Author: Christo
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>

#include "CompetitorInspector.h"
#include "ResultInspector.h"
#include "RoundInspector.h"
#include "ui_ChronoDuino.h"

class ChronoDuino : public QMainWindow
{
	Q_OBJECT

public:
	ChronoDuino(QWidget * a_parent=NULL);
	virtual ~ChronoDuino();

public slots:
	void showMessage(const QString& a_message);

private:
	void initRace();

	Ui::ChronoDuino m_main_window;
	ResultInspector* m_result_inspector;
	CompetitorInspector * m_competitor_inspector;
	RoundInspector * m_round_inspector;

private slots:
	void initDbConnection();
	void newRaceAction();
	void openRaceAction();
	void editPreferencesAction();
	void printAction();
	void testAction();
	void aboutAction();
};

#endif /* MAINWINDOW_H_ */
