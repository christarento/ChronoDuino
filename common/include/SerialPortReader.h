/*
 * SerialPortReader.h
 *
 *  Created on: 6 sept. 2009
 *      Author: Christo
 */

#ifndef SERIALPORTREADER_H_
#define SERIALPORTREADER_H_

class SerialPortReader
{
public:
	virtual void processSerialData(const char a_value) = 0;
};

#endif /* SERIALPORTREADER_H_ */
