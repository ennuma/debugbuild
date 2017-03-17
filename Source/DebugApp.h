#ifndef DEBUGAPP
#define DEBUGAPP
#include "Direct3D11\D3D11App.h" 
#include "GameSceneModule.h"

class DebugApp : public D3D11App
{
	typedef	D3D11App	_parent;
public:
	DebugApp();
    ~DebugApp();
	bool init();

	void exit();

	char* getTitle() const override;
	
	void  drawFrame() override;
private:
	LARGE_INTEGER lastDrawTime;
	float fTickToSec;
	GameSceneModule* m_sceneMod;
};
#endif