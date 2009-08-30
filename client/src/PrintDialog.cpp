
#include <QPrinter>
#include <QPrintDialog>
#include <QSqlQuery>
#include <QTextBlockFormat>

#include "ChronoContext.h"
#include "PrintDialog.h"

PrintDialog::PrintDialog(QWidget* a_parent) :
	QDialog(a_parent)
{
	m_ui.setupUi(this);
	connect(m_ui.m_action_print, SIGNAL(triggered()), this, SLOT(sPrint()));

	writeResults();
}

PrintDialog::~PrintDialog()
{

}

void PrintDialog::sPrint()
{
	QPrinter printer;
	printer.setOutputFormat(QPrinter::PdfFormat);
	QPrintDialog printDialog(&printer, this);
	printDialog.setWindowTitle(tr("Print Document"));
	if (printDialog.exec() != QDialog::Accepted)
		return;

	m_ui.m_text_edit->print(&printer);
}

void PrintDialog::insertTitleResult(QTextCursor & cursor, const QString & raceName)
{
	QTextBlockFormat blockTitleFormat;
	blockTitleFormat.setAlignment(Qt::AlignCenter);
	blockTitleFormat.setTopMargin(0.0);
	blockTitleFormat.setBottomMargin(80.0);

	QTextCharFormat titleFormat;
	titleFormat.setFontPointSize(20);
	titleFormat.setFontWeight(50);
	titleFormat.setFontCapitalization(QFont::AllUppercase);

	cursor.insertBlock(blockTitleFormat);
	cursor.insertText(raceName, titleFormat);
}

void PrintDialog::insertHeaderResult(QTextCursor & cursor, const QString & location, const QString & date)
{
	QTextBlockFormat blockHeaderFormat;
	blockHeaderFormat.setAlignment(Qt::AlignLeft);
	blockHeaderFormat.setTopMargin(5.0);
	blockHeaderFormat.setBottomMargin(0.0);
	blockHeaderFormat.setLeftMargin(20.0);

	QTextCharFormat headerFormat1;
	headerFormat1.setFontPointSize(10);
	headerFormat1.setFontWeight(50);
	QTextCharFormat headerFormat2;
	headerFormat2.setFontPointSize(10);
	headerFormat2.setFontWeight(50);

	cursor.insertBlock(blockHeaderFormat);
	cursor.insertText(tr("Location : "), headerFormat1);
	cursor.insertText(location, headerFormat2);

	cursor.insertBlock(blockHeaderFormat);
	cursor.insertText(tr("Date : "), headerFormat1);
	cursor.insertText(date, headerFormat2);
}

void PrintDialog::insertSeparatorLine(QTextCursor & cursor)
{
	QTextBlockFormat blockSeparatorFormat;
	blockSeparatorFormat.setAlignment(Qt::AlignCenter);
	blockSeparatorFormat.setTopMargin(10.0);
	blockSeparatorFormat.setBottomMargin(10.0);

	QTextCharFormat separatorFormat;
	separatorFormat.setFontCapitalization(QFont::AllUppercase);
	separatorFormat.setFontWeight(50);
	separatorFormat.setFontPointSize(10.0);

	cursor.insertBlock(blockSeparatorFormat);
	cursor.insertText("_______________________________________________",separatorFormat);
}

QTextTable * PrintDialog::insertCategoryTable(QTextCursor & cursor, const QString & categoryName, const unsigned int & rows)
{
	QTextBlockFormat blockCategoryTitleFormat;
	blockCategoryTitleFormat.setAlignment(Qt::AlignCenter);
	blockCategoryTitleFormat.setTopMargin(40.0);
	blockCategoryTitleFormat.setBottomMargin(30.0);

	QTextCharFormat categoryTitleFormat;
	categoryTitleFormat.setFontCapitalization(QFont::AllUppercase);
	categoryTitleFormat.setFontWeight(25);
	categoryTitleFormat.setFontPointSize(14.0);
	QString category = "Category \"";
	category += categoryName;
	category += "\"";

	cursor.insertBlock(blockCategoryTitleFormat);
	cursor.insertText(category,categoryTitleFormat);


	cursor.insertBlock(QTextBlockFormat()); // to break the previous block format
	QTextTable * table = cursor.insertTable(rows+1, 4);
	QTextTableFormat categoryTableFormat;
	categoryTableFormat.setAlignment(Qt::AlignHCenter);
	categoryTableFormat.setHeaderRowCount(1); // header line
	categoryTableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
	categoryTableFormat.setBorder(1.0);
	categoryTableFormat.setCellPadding(10);
	categoryTableFormat.setCellSpacing(0);

	table->setFormat(categoryTableFormat);

	// header :
	// header cell format :
	QTextCharFormat headerCellFormat;
	headerCellFormat.setFontWeight(50);
	headerCellFormat.setFontPointSize(12.0);
	headerCellFormat.setBackground(QBrush(QColor(60,60,60)));
	headerCellFormat.setForeground(QBrush(QColor(255,255,255)));

	QTextTableCell cell = table->cellAt(0,0);
	cell.setFormat(headerCellFormat);
	cursor = cell.firstCursorPosition();
	cursor.insertText(tr("Position"));

	cell = table->cellAt(0,1);
	cell.setFormat(headerCellFormat);
	cursor = cell.firstCursorPosition();
	cursor.insertText(tr("LastName"));

	cell = table->cellAt(0,2);
	cell.setFormat(headerCellFormat);
	cursor = cell.firstCursorPosition();
	cursor.insertText(tr("Firstname"));

	cell = table->cellAt(0,3);
	cell.setFormat(headerCellFormat);
	cursor = cell.firstCursorPosition();
	cursor.insertText(tr("Time"));

	return table;
}

void PrintDialog::fillCategoryTableLine(  QTextTable * table, const unsigned int & row,
		const QString & position, const QString & lastname,
		const QString & firstname, const QString & time)
{
	int realRow = row + 1;

	QTextCharFormat cellFormat;
	cellFormat.setFontPointSize(10.0);
	if(row%2)
		cellFormat.setBackground(QBrush(QColor(240,240,240)));

	QTextTableCell cell = table->cellAt(realRow, 0);
	QTextCursor cursor = cell.firstCursorPosition();
	cell.setFormat(cellFormat);
	cursor.insertText(position);

	cell = table->cellAt(realRow, 1);
	cursor = cell.firstCursorPosition();
	cell.setFormat(cellFormat);
	cursor.insertText(lastname);

	cell = table->cellAt(realRow, 2);
	cursor = cell.firstCursorPosition();
	cell.setFormat(cellFormat);
	cursor.insertText(firstname);

	cell = table->cellAt(realRow, 3);
	cursor = cell.firstCursorPosition();
	cell.setFormat(cellFormat);
	cursor.insertText(time);
}

void PrintDialog::writeResults()
{
	QTextCursor cursor(m_ui.m_text_edit->textCursor());
	cursor.movePosition(QTextCursor::Start);

	//Race id
	const int race_id = CHRONOCONTEXT.getCurrentRace();

	//Race query
	QSqlQuery queryRaceName;
	queryRaceName.prepare("SELECT name, place, date FROM t_races "
			"WHERE id=:race_id");
	queryRaceName.bindValue(":race_id", race_id);
	if (!queryRaceName.exec() || !queryRaceName.next())
		return;

	const QString race_name = queryRaceName.value(0).toString();
	const QString race_place = queryRaceName.value(1).toString();
	const QString race_date = queryRaceName.value(2).toString();

	insertTitleResult(cursor, race_name);
	insertHeaderResult(cursor, race_place, race_date);


	//Category query
	QSqlQuery category_query;
	category_query.prepare("SELECT id, name FROM t_categories "
			"WHERE race_id=:race_id");
	category_query.bindValue(":race_id", race_id);
	if (!category_query.exec())
		return;
	while(category_query.next())
	{
		const QString category_id = category_query.value(0).toString();
		const QString category_name = category_query.value(1).toString();

		//Result query
		QSqlQuery result_query;
		result_query.prepare("SELECT res.time, pe.first_name, pe.last_name FROM t_results res"
				"JOIN t_registrations reg ON res.registration_id=reg.id "
				"JOIN t_persons pe ON reg.person_id=pe.id "
				"JOIN t_categories cat ON reg.category_id=cat.id WHERE cat.race_id=:race_id "
				"ORDER BY res.time ASC");
		result_query.bindValue(":race_id", race_id);

		if (!result_query.exec())
			return;

		int rows = result_query.size();
		cursor = m_ui.m_text_edit->textCursor();
		insertSeparatorLine(cursor);
		QTextTable * tableCategory = insertCategoryTable(cursor, category_name, rows);

		int pos = 1;
		while(result_query.next())
		{
			//Draw
			const QString result_time = result_query.value(0).toString();
			const QString result_firstname = result_query.value(1).toString();
			const QString result_lastname = result_query.value(2).toString();
			fillCategoryTableLine(tableCategory, pos-1, QString::number(pos), result_lastname, result_firstname, result_time);
			++pos;
		}
	}
}
