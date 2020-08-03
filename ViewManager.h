#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <set>
#include <memory>

#include "View.h"

typedef std::shared_ptr<View> view_ptr;

class ViewManager
{
public:
	ViewManager();

	void push(view_ptr view);
	void pop(view_ptr view);

	void launchViews(void);
	void pollInput(void);

private:
	std::set<view_ptr> _views;

};

#endif // VIEWMANAGER_H
