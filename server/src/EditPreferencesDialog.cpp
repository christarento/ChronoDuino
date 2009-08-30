/*
 * EditPreferencesDialog.cpp
 *
 *  Created on: 9 août 2009
 *      Author: Christo
 */

#include <QComboBox>
#include <QSettings>
#include <QSpinBox>

#include "EditPreferencesDialog.h"
#include "SerialPort.h"

const QString EditPreferencesDialog::BIND_PORT = "bind_port";
const QString EditPreferencesDialog::SERIAL_PORT = "serial/port";
const QString EditPreferencesDialog::SERIAL_RATE = "serial/rate";

EditPreferencesDialog::EditPreferencesDialog(QWidget* a_parent):
	QDialog(a_parent)
{
	//UI
	m_dialog.setupUi(this);

	//Read from QSettings
	const QSettings settings;

	//Bind port
	m_dialog.m_sb_listening_port->setValue( settings.value(BIND_PORT, 8000).toInt() );

	//Serial com
	QStringList serial_list = SerialPort::listSerialPort();
	m_dialog.m_cb_serial_port->addItems(serial_list);
	const int port_idx = m_dialog.m_cb_serial_port->findText( settings.value(SERIAL_PORT).toString() );
	if (port_idx != -1)
		m_dialog.m_cb_serial_port->setCurrentIndex(port_idx);

	const int rate_idx = m_dialog.m_cb_serial_rate->findText( settings.value(SERIAL_RATE, 9600).toString() );
	if (rate_idx != -1)
		m_dialog.m_cb_serial_rate->setCurrentIndex(rate_idx);

	//Connect
	connect(m_dialog.m_button_box, SIGNAL(accepted()), SLOT(onAccept()));
}

EditPreferencesDialog::~EditPreferencesDialog()
{

}

void EditPreferencesDialog::onAccept()
{
	QSettings settings;

	//Bond port
	settings.setValue(BIND_PORT, m_dialog.m_sb_listening_port->value());

	//Serial com
	if (m_dialog.m_cb_serial_port->count())
		settings.setValue(SERIAL_PORT, m_dialog.m_cb_serial_port->currentText());
	settings.setValue(SERIAL_RATE, m_dialog.m_cb_serial_rate->currentText());

	accept();
}
