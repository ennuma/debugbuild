#ifndef GAMESCENEMODULE
#define GAMESCENEMODULE
#include <time.h>
#include <vector>
class Region;
class Actor;
class GameSceneModule;
enum EActionType
{
	EAction_Move,
	EAction_Duel
};

struct Action
{
	EActionType eAction;
	Actor* pOrigActor;
	float fExecutionTime;
};

struct MoveAction : Action
{
	MoveAction()
	{
		eAction = EActionType::EAction_Move;
	}
	Region* pDestRegion;
};

struct DuelAction : Action
{
	DuelAction()
	{
		eAction = EActionType::EAction_Duel;
	}
	Actor* pDestActor;
};

class Actor
{
public:
	Actor();
	~Actor();
	std::vector<std::pair<Actor*, int>> relationKinsMap;
	std::vector<std::pair<Actor*, int>> relationMap;
	float timeTillNextUpdate;
	void Update(time_t deltaTime); // assign timeTillNextUpdate
	Region* m_currentRegion;
	void (GameSceneModule::*Callback_AddAction)(Action* in_pAction); //define function pointer to member func
	bool isAlive;
};

class Region
{
public:
	Region();
	~Region();
	int m_regionId;
	std::vector<Region*> m_adjRegions;
	std::vector<Actor*> m_withinActors;
};

namespace gamescenehelper
{
	void AssignActorToRandomRegion(Actor* pAction, std::vector<Region*> &regionList);
};

class GameSceneModule
{
public:
	GameSceneModule();
	~GameSceneModule();
	void WorldTick(time_t deltaTime);
	void AddAction(Action* in_pAction);
private:
	std::vector<Actor*> m_actorUpdateList;
	std::vector<Actor*> m_actorPool;
	std::vector<Region*> m_regionPool;
	std::vector<Action*> m_actionExecutionList;
	void ExecuteAction(Action* in_pAction, time_t in_deltaTime);
	//bool ActionIsValid(Action* in_pAction);
};
#endif