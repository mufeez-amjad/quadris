#ifndef SUBJECT_H
#define SUBJECT_H

#include <set>
#include <mutex>

#include "Observer.h"

class Subject
{
public:
	virtual ~Subject() {};
	void subscribe(Observer*);
	void unsubscribe(Observer*);

protected:
	void _notify(void);

private:
	std::mutex _lock;
	std::set<Observer*> _observers;
};

#endif // SUBJECT_H
