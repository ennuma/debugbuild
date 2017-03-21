#include "GameSceneModule.h"
#include <random>
#include <algorithm>
#include <assert.h>
extern GameSceneModule* g_sceneMod;

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
		//assigne callback func
		pActor->Callback_AddAction = &(g_sceneMod->AddAction);
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
		var->Update(deltaTime);
	}
	for each (auto var in m_actionExecutionList)
	{
		ExecuteAction(var, deltaTime);
	}
}

//bool GameSceneModule::ActionIsValid(Action* in_pAction)
//{
//	return true;
//}


void GameSceneModule::ExecuteAction(Action* in_pAction, time_t in_deltaTime)
{
	//if (!ActionIsValid(in_pAction))
	{
		delete in_pAction;
		return;
	}
	in_pAction->fExecutionTime -= in_deltaTime;
	if (in_pAction->fExecutionTime < 0)
	{
		switch (in_pAction->eAction)
		{
		case EAction_Move:
			in_pAction->pOrigActor->m_currentRegion->m_withinActors.erase(
				std::remove(
					in_pAction->pOrigActor->m_currentRegion->m_withinActors.begin(),
					in_pAction->pOrigActor->m_currentRegion->m_withinActors.end(),
					in_pAction->pOrigActor));
			in_pAction->pOrigActor->m_currentRegion = static_cast<MoveAction*>(in_pAction)->pDestRegion;
			static_cast<MoveAction*>(in_pAction)->pDestRegion->m_withinActors.push_back(in_pAction->pOrigActor);
			break;
		case EAction_Duel:
			break;
		default:
			break;
		}
		delete in_pAction;
	}
}

void GameSceneModule::AddAction(Action* in_pAction)
{
	m_actionExecutionList.push_back(in_pAction);
}

//Actor class
Actor::Actor()
{
	isAlive = true;
}

Actor::~Actor()
{

}

void Actor::Update(time_t deltaTime)
{
	if (!isAlive)
	{
		return;
	}
	//TODO create action and assign timeTillNextUpdate
	int randInt = rand();
	float randZero2One = float(randInt) / float(RAND_MAX);
	if (randZero2One > 0.5f)
	{
		//create move command
		MoveAction* move = new MoveAction();
		move->pOrigActor = this;
		Region* dest = this->m_currentRegion->m_adjRegions[0];
		move->pDestRegion = dest;
		move->fExecutionTime = 5.0f;
		if (Callback_AddAction != nullptr)
		{
			(g_sceneMod->*Callback_AddAction)(move);
		}
		else
		{
			//callback not assigned
			assert(0);
		}
	}
	else
	{
		//create dual command
		DuelAction* dual = new DuelAction();
		dual->pOrigActor = this;
		Actor* pDestActor = this->m_currentRegion->m_withinActors[0];
		dual->pDestActor = pDestActor;
		dual->fExecutionTime = 2.0f;
		if (Callback_AddAction != nullptr)
		{
			(g_sceneMod->*Callback_AddAction)(dual);
		}
		else
		{
			//callback not assigned
			assert(0);
		}
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