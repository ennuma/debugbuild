#include "GameSceneModule.h"

GameSceneModule::GameSceneModule()
{
	for (int i = 0; i < 100; i++)
	{
		Actor* pActor = new Actor();
		actorPool.push_back(pActor);
		actorUpdateList.push_back(pActor);
	}
}

GameSceneModule::~GameSceneModule()
{
	for each (Actor* pActor in actorPool)
	{
		delete pActor;
	}
}

void GameSceneModule::WorldTick(time_t deltaTime)
{


}

//Actor class
Actor::Actor()
{

}

Actor::~Actor()
{

}

//Region class
Region::Region()
{

}

Region::~Region()
{

}