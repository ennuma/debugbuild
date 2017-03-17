#ifndef GAMESCENEMODULE
#define GAMESCENEMODULE
#include <time.h>
class GameSceneModule
{
public:
	GameSceneModule();
    ~GameSceneModule();
	void WorldTick(time_t deltaTime);
private:

};
#endif