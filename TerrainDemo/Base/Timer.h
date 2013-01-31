/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <boost/chrono/chrono_io.hpp>

#define TIMER_NOW boost::chrono::high_resolution_clock::now()

typedef boost::chrono::high_resolution_clock::time_point time_point;

//! Timer
/*!
	...
 */
class CTimer {
public:
	CTimer(void) {
		m_tpStart = TIMER_NOW;
		m_tpStop = TIMER_NOW;
	}
	~CTimer(void) {}
	void start(void) {
		m_tpStart = TIMER_NOW;
	}
	void stop(void) {
		m_tpStop = TIMER_NOW;
	}
	time_point getStart(void) {
		return m_tpStart;
	}
	time_point getStop(void) {
		return m_tpStop;
	}
protected:
private:
	time_point m_tpStart;
	time_point m_tpStop;
};
#endif