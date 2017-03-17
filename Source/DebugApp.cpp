#include "DebugApp.h"

//init global app that gets operated in main loop
BaseApp *app = new DebugApp();

DebugApp::DebugApp()
{

}

DebugApp::~DebugApp()
{

}

char* DebugApp::getTitle() const
{
	return "DebugTestApp";
}

void  DebugApp::drawFrame()
{

}