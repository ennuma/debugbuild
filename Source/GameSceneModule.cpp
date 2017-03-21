#include "GameSceneModule.h"
#include <random>
#include <algorithm>
#include <assert.h>
extern GameSceneModule* g_sceneMod;

void InfoMsg(const char *string, ...) {
	ASSERT(string);

	//Eval the string
	const unsigned BUFFER_SIZE = 1024;
	char buf[BUFFER_SIZE];

	va_list arglist;
	va_start(arglist, string);
	vsprintf_s(buf, BUFFER_SIZE, string, arglist);
	va_end(arglist);
	{
		outputDebugString(buf);
	}
}

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

	for (int i = 0; i < 10; i++)
	{
		Actor* pActor = new Actor();
		pActor->m_actorId = i;
		pActor->m_strength = (int) (100.0f*(float(rand())/float(RAND_MAX))); //0-100
		m_actorPool.push_back(pActor);
		m_actorUpdateList.push_back(pActor);
		gamescenehelper::AssignActorToRandomRegion(pActor, m_regionPool);
		//assigne callback func
		pActor->Callback_AddAction = &GameSceneModule::AddAction;
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

void GameSceneModule::WorldTick(float deltaTime)
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

bool GameSceneModule::ActionIsValid(Action* in_pAction)
{
	switch (in_pAction->eAction)
	{
	case EAction_Move:
	{
		MoveAction* pMove = static_cast<MoveAction*>(in_pAction);
		if (!pMove->pOrigActor->isAlive)
			return false;
		break;
	}
	case EAction_Duel:
	{
		DuelAction* pDuel = static_cast<DuelAction*>(in_pAction);
		if (!pDuel->pOrigActor->isAlive || !pDuel->pDestActor->isAlive)
			return false;
		break;
	}
	default:
		break;
	}
	return true;
}


void GameSceneModule::ExecuteAction(Action* in_pAction, float in_deltaTime)
{
	if (!ActionIsValid(in_pAction))
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
		{
			MoveAction* pMove = static_cast<MoveAction*>(in_pAction);

			pMove->pOrigActor->m_currentRegion->m_withinActors.erase(
				std::remove(
					pMove->pOrigActor->m_currentRegion->m_withinActors.begin(),
					pMove->pOrigActor->m_currentRegion->m_withinActors.end(),
					pMove->pOrigActor));
			pMove->pOrigActor->m_currentRegion = pMove->pDestRegion;
			pMove->pDestRegion->m_withinActors.push_back(in_pAction->pOrigActor);

			InfoMsg("Actor Id %i :	Move to	Region Id %i\n",
				in_pAction->pOrigActor->m_actorId,
				pMove->pDestRegion->m_regionId);
			break;
		}
		case EAction_Duel:
		{
			DuelAction* pDuel = static_cast<DuelAction*>(in_pAction);
			if (pDuel->pOrigActor->m_strength > pDuel->pDestActor->m_strength)
			{
				pDuel->pDestActor->isAlive = false;
				pDuel->pDestActor->m_currentRegion->m_withinActors.erase(
					std::remove(
						pDuel->pOrigActor->m_currentRegion->m_withinActors.begin(),
						pDuel->pOrigActor->m_currentRegion->m_withinActors.end(),
						pDuel->pOrigActor));
				InfoMsg("Actor Id %i vs Actor Id %i :	Actor Id %i dead\n",
					pDuel->pOrigActor->m_actorId,
					pDuel->pDestActor->m_actorId,
					pDuel->pDestActor->m_actorId);
			}
			else
			{
				pDuel->pOrigActor->isAlive = false;
				pDuel->pOrigActor->m_currentRegion->m_withinActors.erase(
					std::remove(
						pDuel->pOrigActor->m_currentRegion->m_withinActors.begin(),
						pDuel->pOrigActor->m_currentRegion->m_withinActors.end(),
						pDuel->pOrigActor));
				InfoMsg("Actor Id %i vs Actor Id %i :	Actor Id %i dead\n",
					pDuel->pOrigActor->m_actorId,
					pDuel->pDestActor->m_actorId,
					pDuel->pOrigActor->m_actorId);
			}
		}
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

void Actor::Update(float deltaTime)
{
	timeTillNextUpdate -= deltaTime;
	if (!isAlive || timeTillNextUpdate>0)
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

	//reset timer and queue for next action
	timeTillNextUpdate = float(rand()) / float(RAND_MAX) * 4.0f;
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
	size_t randIdx = size_t(size * (float(randInt) / float(RAND_MAX)));
	Region* randRegion = regionList[randIdx];

	//remove actor from previous region
	Region* prevRegion = pActor->m_currentRegion;
	if(prevRegion!=nullptr)
		prevRegion->m_withinActors.erase(
		std::remove(prevRegion->m_withinActors.begin(),
			prevRegion->m_withinActors.end(),
			pActor));

	//add actor to new region
	randRegion->m_withinActors.push_back(pActor);

	//assign current region
	pActor->m_currentRegion = randRegion;
}