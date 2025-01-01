#include "FSMManager.h"
#include <utility>
#include "FSM.h"
#include "ObjectManager.h"
#include "System.h"

uint32_t FSMManager::lastFSMHandle_ = 1;

FSMManager::FSMManager()
{
	eventHandle_LevelEnd_ = System::GetInstance()->
		Event_LevelEnd.Subscribe(BindSubscriber(&FSMManager::CleanUpOldFSMs, this));
}

FSMManager::~FSMManager()
{
	System::GetInstance()->Event_LevelEnd.Unsubscribe(eventHandle_LevelEnd_);
}

void FSMManager::CleanUpOldFSMs()
{
	fsmList_.clear();
}

std::weak_ptr<FSM> FSMManager::CreateFSM(std::weak_ptr<GameEntity> entity)
{
	auto weakFsm = System::GetInstance()->GetObjectMgr()->CreateObject<FSM>();

	if(auto strongFsm = weakFsm.lock())
	{
		strongFsm->SetEntity(std::move(entity));
		strongFsm->SetHandle(lastFSMHandle_++);
	}
	fsmList_.push_back(weakFsm);

	return weakFsm;
}

std::weak_ptr<FSM> FSMManager::GetFSM(uint32_t handle)
{
	auto result = FindFSM(handle);
	assert(result != fsmList_.end() && "FSM with specified handle not found");
	return *result;
}

std::vector<std::weak_ptr<FSM>>::iterator FSMManager::FindFSM(const int handle)
{
	return std::find_if(fsmList_.begin(),
						fsmList_.end(),
						[&handle](std::weak_ptr<FSM>& fsm) -> bool
						{
							return fsm.lock()->GetHandle() == handle;
						});
}

void FSMManager::UpdateFSMs(float deltaTime)
{
	for (auto& fsm_ : fsmList_)
	{
		if (auto fsm = fsm_.lock())
		{
			fsm->Update(deltaTime);
		}
	}
}

void FSMManager::KillFSM(int handle)
{
	// this removes it from the fsm list,
	// object manager already takes care of object lifetime
	fsmList_.erase(FindFSM(handle));
}