#pragma once

#include "pch.h"

class FSM;
class GameEntity;

class FSMManager
{
public:
	FSMManager();
	~FSMManager();

	std::weak_ptr<FSM> CreateFSM(std::weak_ptr<GameEntity> entity);
	std::weak_ptr<FSM> GetFSM(uint32_t handle);

	void UpdateFSMs(float deltaTime);

	void KillFSM(int handle);

private:
	void CleanUpOldFSMs();
	std::vector<std::weak_ptr<FSM>>::iterator FindFSM(int handle);
	
	static uint32_t lastFSMHandle_;

	uint32_t eventHandle_LevelEnd_ = 0;

	std::vector<std::weak_ptr<FSM>> fsmList_;
};
