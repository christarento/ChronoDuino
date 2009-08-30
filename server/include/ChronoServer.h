/*
 * ChronoServer.h
 *
 *  Created on: 8 août 2009
 *      Author: Christo
 */

#ifndef CHRONOSERVER_H_
#define CHRONOSERVER_H_

#include <QTcpServer>
#include <QTcpSocket>

#include "ui_ChronoServer.h"

class ChronoServer : public QMainWindow
{
	Q_OBJECT

public:
	static const int MAX_BUFFER_SIZE = 1024000;
	static const char SEPARATOR_TOKEN = '\\';

	enum DataType
	{
		COMPETITOR_INFO,
		COMPETITOR_PHOTO,
		TIME,
		UNDEFINED
	};

	enum State
	{
		WAITING_FOR_CONNECTION,
		WAITING_FOR_COMPETITOR,
		READY,
		ARMED,
		RUNNING,
		FINISHED
	};

	ChronoServer(QWidget* a_parent=NULL);
	virtual ~ChronoServer();

private:
	Ui::ChronoServer m_chrono_server;
	QTcpServer* m_server;
	QTcpSocket* m_socket;

	State m_state;
	DataType m_current_data_type;
	int m_num_bytes_to_read;
	QByteArray m_buffer;

private slots:
	void aboutAction();
	void listenAction();
	void preferencesAction();
	void testAction();

	void sendMessage(const QString& a_message);

	void createNewConnection();
	void reset();
	void processReadyRead();
	bool readProtocolHeader();
	int readDataIntoBuffer(const int a_max_size=MAX_BUFFER_SIZE);
	int dataLengthForCurrentDataType();
	bool hasEnoughData();
	void processData();
};

#endif /* CHRONOSERVER_H_ */
