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
	std::vector<Actor*> m_actorUpdateList;
	std::vector<Actor*> m_actorPool;
	std::vector<Region*> m_regionPool;
};

class Actor
{
public:
	Actor();
	~Actor();
	std::vector<std::pair<Actor*, int>> relationKinsMap;
	std::vector<std::pair<Actor*, int>> relationMap;
	float timeTillNextUpdate;
	void Update(); // assign timeTillNextUpdate
	Region* m_currentRegion;
};

class Region
{
public:
	Region();
	~Region();
	int m_regionId;
	std::vector<Region*> m_adjRegions;
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