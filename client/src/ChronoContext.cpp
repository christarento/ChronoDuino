/*
 * ChronoContext.cpp
 *
 *  Created on: 19 ao�t 2009
 *      Author: Christo
 */

#include "ChronoContext.h"

ChronoContext* ChronoContext::m_instance = 0;

ChronoContext::ChronoContext() :
	m_current_race(-1)
{

}

ChronoContext::~ChronoContext()
{

}
