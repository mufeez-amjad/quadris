#ifndef OBSERVER_H
#define OBSERVER_H

class Observer
{
public:
	virtual ~Observer() {}
	virtual void update(void) const = 0;
};

#endif // OBSERVER_H
