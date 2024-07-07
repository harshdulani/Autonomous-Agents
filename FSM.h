#pragma once
#include <string>
#include <memory>
#include <map>
#include "Graph.h"
#include "FSMParameter.h"
#include "Object.h"
#include "TransitionEdge.h"
#include "StateNode.h"

class GameEntity;

class FSM : public Object
{
public:
	void SetEntity(std::weak_ptr<GameEntity> entity);

	void SetParameter(const std::string& name, int value);
	void SetParameter(const std::string& name, float value);
	void SetParameter(const std::string& name, bool value);

	int GetInt(const std::string& name);
	float GetFloat(const std::string& name);
	bool GetBool(const std::string& name);

	template<typename T>	std::weak_ptr<T> CreateNode();

	template<typename T>	std::weak_ptr<TransitionEdge> CreateEdge(std::weak_ptr<T> src, std::weak_ptr<T> dest);

	void AssignDefaultState(std::weak_ptr<StateNode> state);

	void SetHandle(int handle);
	int GetHandle() const;

	//Called by FSM Manager
	void Initialise();
	void Update(float deltaTime);

private:
	FSMParameter* FindParameterNamed(std::string name);

	FSMParameter* CreateParameterNamed(std::string name, FSMParameter& param);

	static void MakeStringLowercase(std::string& target);

	void TryTransitions();
	void ChangeStateTo(const std::shared_ptr<StateNode>& state);

	Graph<StateNode, TransitionEdge> stateGraph_;
	std::weak_ptr<StateNode> currentState_;
	std::weak_ptr<StateNode> defaultState_;

	std::map<std::string, FSMParameter> globalParameters;

	std::weak_ptr<GameEntity> entity_;
	int myHandle_ = -1;
};

template<typename T> std::weak_ptr<T> FSM::CreateNode()
{
	return stateGraph_.CreateNode<T>();
}

template<typename T> std::weak_ptr<TransitionEdge> FSM::CreateEdge(std::weak_ptr<T> src, std::weak_ptr<T> dest)
{
	return stateGraph_.CreateEdge(src, dest);
}
