#ifndef GAMESCENEMODULE
#define GAMESCENEMODULE
#include <time.h>
#include <vector>
class GameSceneModule
{
public:
	GameSceneModule();
    ~GameSceneModule();
	void WorldTick(time_t deltaTime);
private:
	std::vector<Actor*> actorUpdateList;
	std::vector<Actor*> actorPool;
};

class Actor
{
public:
	Actor();
	~Actor();
};

class Region
{
public:
	Region();
	~Region();
};

enum EActionType
{
	EAction_Move,
	EAction_Duel
};

struct Action
{
	EActionType eAction;
	Actor* pOrigActor;
};

struct MoveAction : Action
{
	MoveAction(Region* _destRegion)
	{
		eAction = EActionType::EAction_Move;
		pDestRegion = _destRegion;
	}
	Region* pDestRegion;
};

struct DuelAction : Action
{
	DuelAction(Actor* _actor)
	{
		eAction = EActionType::EAction_Duel;
		pDestActor = _actor;
	}
	Actor* pDestActor;
};
#endif