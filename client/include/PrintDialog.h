#ifndef DIALOGPRINT_H
#define DIALOGPRINT_H

#include <QDialog>
#include <QTextTable>
#include <QTextCursor>


#include "ui_PrintDialog.h"

class PrintDialog : public QDialog
{
    Q_OBJECT
public:
    PrintDialog(QWidget* a_parent=NULL);
    ~PrintDialog();

    void writeResults();

public slots :
    void sPrint();

private:
    // write text :
    static QTextTable * insertCategoryTable(QTextCursor & cursor, const QString & categoryName, const unsigned int & rows);
    static void fillCategoryTableLine(
    		QTextTable * table, const unsigned int & row,
    		const QString & position, const QString & lastname,
    		const QString & firstname, const QString & time);
    static void insertTitleResult(QTextCursor & cursor, const QString & raceName);
    static void insertHeaderResult(QTextCursor & cursor, const QString & location, const QString & date);
    static void insertSeparatorLine(QTextCursor & cursor);

private:
    Ui::PrintDialog m_ui;
};

#endif // DIALOGPRINT_H
