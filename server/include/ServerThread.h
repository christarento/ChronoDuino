/*
 * ServerThread.h
 *
 *  Created on: 11 sept. 2009
 *      Author: christo
 */

#ifndef SERVERTHREAD_H_
#define SERVERTHREAD_H_

#include <QTcpSocket>
#include <QThread>

class ServerThread : public QThread
{
	Q_OBJECT
public:
	ServerThread(QObject* a_parent=NULL);
	virtual ~ServerThread();

	void open(const int a_descriptor);
	void arm();
	void stopChrono();

signals:
	void connected();
	void error(const QString&);

	void competitorArmed(
			const QString& a_first_name,
			const QString& a_last_name,
			const QString& a_category);
	void currentTime(const int&);

protected:
	virtual void run();

private:
	static const int MAX_BUFFER_SIZE = 1024000;
	static const char SEPARATOR_TOKEN = '\\';

	enum DataType
	{
		COMPETITOR_INFO,
		COMPETITOR_PHOTO,
		TIME,
		UNDEFINED
	};

	void process();
	bool readProtocolHeader();
	int readDataIntoBuffer(const int a_max_size=MAX_BUFFER_SIZE);
	int dataLengthForCurrentDataType();
	bool hasEnoughData();

	//Socket
	QTcpSocket* m_socket;
	int m_descriptor;

	//Input stream
	DataType m_current_data_type;
	int m_num_bytes_to_read;
	QByteArray m_buffer;

private slots:
	void processData();
	void socketError(QAbstractSocket::SocketError);
	void socketConnected();
};

#endif /* SERVERTHREAD_H_ */
