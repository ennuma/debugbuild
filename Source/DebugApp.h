#ifndef DEBUGAPP
#define DEBUGAPP
#include "Direct3D11\D3D11App.h" 
//init global app that gets operated in main loop
BaseApp *app = new DebugApp();

class DebugApp : public D3D11App
{
public:
	DebugApp();
    ~DebugApp();
	char* getTitle() const override;
	void  drawFrame() override;
private:

};
#endif