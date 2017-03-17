#include "DebugApp.h"

//init global app that gets operated in main loop
BaseApp *app = new DebugApp();

DebugApp::DebugApp()
{

}

DebugApp::~DebugApp()
{

}

bool DebugApp::init()
{
	if (!_parent::initAPI()) return false;
	m_sceneMod = new GameSceneModule();
	//StarLab
	QueryPerformanceCounter(&lastDrawTime);
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	fTickToSec = 1.f / float(freq.QuadPart);
	return true;
}

void DebugApp::exit()
{
	delete m_sceneMod;
	_parent::exitAPI();
}

char* DebugApp::getTitle() const
{
	return "DebugTestApp";
}

void  DebugApp::drawFrame()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LONGLONG iDelta = currentTime.QuadPart - lastDrawTime.QuadPart;
	lastDrawTime = currentTime;
	float fDeltaTime = float(iDelta) * fTickToSec;
	m_sceneMod->WorldTick(fDeltaTime);
}