/*
 * ChronoContext.h
 *
 *  Created on: 19 août 2009
 *      Author: Christo
 */

#ifndef CHRONOCONTEXT_H_
#define CHRONOCONTEXT_H_

#define CHRONOCONTEXT (ChronoContext::getInstance())

class ChronoContext
{
public:
	static ChronoContext& getInstance()
	{
		if (!m_instance)
			m_instance = new ChronoContext;
		return *m_instance;
	}
	static void release()
	{
		delete m_instance;
		m_instance = 0;
	}

	int getCurrentRace() const {return m_current_race;}
	void setCurrentRace(const int a_race_index) {m_current_race = a_race_index;}

private:
	ChronoContext();
	ChronoContext(const ChronoContext&) {}
	~ChronoContext();

	static ChronoContext* m_instance;

	int m_current_race;
};

#endif /* CHRONOCONTEXT_H_ */
