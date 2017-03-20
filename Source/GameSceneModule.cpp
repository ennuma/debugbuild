#include "GameSceneModule.h"
#include <random>
#include <algorithm>
GameSceneModule::GameSceneModule()
{
	for (int i = 0; i < 4; i++)
	{
		Region* pRegion = new Region();
		pRegion->m_regionId = i;
		m_regionPool.push_back(pRegion);
	}
	for (int i = 0; i < 4; i++)
	{
		Region* pCurRegion = m_regionPool[i];
		for (int j = 0; j < 4; j++)
		{
			if (j != i)
			{
				Region* pAdjRegion = m_regionPool[j];
				pCurRegion->m_adjRegions.push_back(pAdjRegion);
			}
		}
	}

	for (int i = 0; i < 100; i++)
	{
		Actor* pActor = new Actor();
		m_actorPool.push_back(pActor);
		m_actorUpdateList.push_back(pActor);
		gamescenehelper::AssignActorToRandomRegion(pActor, m_regionPool);
	}	
}

GameSceneModule::~GameSceneModule()
{
	for each (Actor* pActor in m_actorPool)
	{
		delete pActor;
	}
	for each (Region* pRegion in m_regionPool)
	{
		delete pRegion;
	}
}

void GameSceneModule::WorldTick(time_t deltaTime)
{
	for each (auto var in m_actorUpdateList)
	{

	}
}

//Actor class
Actor::Actor()
{

}

Actor::~Actor()
{

}

void Actor::Update()
{
	//TODO create action and assign timeTillNextUpdate
	int randInt = rand();
	float randZero2One = randInt / RAND_MAX;
	if (randZero2One > 0.5f)
	{
		//create move command
		MoveAction move;
		move.pOrigActor = this;
	}
	else
	{
		//create dual command
	}
}
//Region class
Region::Region()
{

}

Region::~Region()
{

}


//helper funcitons
void gamescenehelper::AssignActorToRandomRegion(Actor* pActor, std::vector<Region*> &regionList)
{
	//get random current region
	size_t size = regionList.size();
	int randInt = rand();
	size_t randIdx = size * (randInt / RAND_MAX);
	Region* randRegion = regionList[randIdx];

	//remove actor from previous region
	Region* prevRegion = pActor->m_currentRegion;
	prevRegion->m_withinActors.erase(
		std::remove(prevRegion->m_withinActors.begin(),
			prevRegion->m_withinActors.end(),
			pActor));

	//add actor to new region
	randRegion->m_withinActors.push_back(pActor);

	//assign current region
	pActor->m_currentRegion = randRegion;
}