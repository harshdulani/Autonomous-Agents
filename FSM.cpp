#include "FSM.h"

#include <algorithm>
#include <utility>

void FSM::SetEntity(std::weak_ptr<GameEntity> owningEntity)
{
	entity = std::move(owningEntity);
}

int FSM::GetHandle() const { return myHandle_; }

void FSM::SetHandle(int handle)
{
	myHandle_ = handle;
}

void FSM::AssignDefaultState(std::weak_ptr<StateNode> state)
{
	defaultState_ = std::move(state);
}

void FSM::Update(float deltaTime)
{
	if (auto currentState = currentState_.lock())
	{
		currentState->UpdateState(deltaTime);
	}
	TryTransitions();
}

void FSM::MakeStringLowercase(std::string& target)
{
	std::transform(target.begin(), target.end(), target.begin(), std::tolower);
}

void FSM::TryTransitions()
{
	auto currentState = currentState_.lock();
	if (!currentState)
	{
		return;
	}

	// go through all the transitions from this state/ node
	for (auto& edge: currentState->GetEdges())
	{
		if(auto transition = std::dynamic_pointer_cast<TransitionEdge>(edge.lock()))
		{
			bool shouldExecuteTransition = true;
			// go through all the conditions required to activate this transition
			for (auto& condition : transition->GetTransitionConditions())
			{
				auto compareWith = FindParameterNamed(condition.GetConditionParamName());
				assert(compareWith != nullptr && "Invalid parameter");
				if (!condition.DoesConditionMatch(compareWith))
				{
					shouldExecuteTransition = false;
					break;
				}
			}

			//if you don't match any of the above conditions, you have broken out of the for loop
			//you will only get here if you match all conditions for a transition
			if (shouldExecuteTransition)
			{
				if (auto currState = currentState_.lock())
				{
					currState->OnExitState();
				}

				if (auto destState = std::dynamic_pointer_cast<StateNode>(transition->GetDestNode().lock()))
				{
					destState->OnEnterState();
					ChangeStateTo(destState);
					return;
				}
			}
		}
	}
}

void FSM::ChangeStateTo(const std::shared_ptr<StateNode>& state)
{
	assert(state != nullptr && "Destination state is nullptr");
	// search using state* in graph to get original shared ptr to state
	auto newState = std::dynamic_pointer_cast<StateNode>(*stateGraph_.FindNodeSharedPtr(state.get()));

	currentState_ = newState;
}

void FSM::SetParameter(const std::string& name, int value)
{
	auto param = FindParameterNamed(name);
	if (param == nullptr)
	{
		param = CreateParameterNamed(name, FSMParameter(value));
	}
	param->AssertParameterInt();
	param->GetValue().iVal = value;
}

void FSM::SetParameter(const std::string& name, float value)
{
	auto param = FindParameterNamed(name);
	if (param == nullptr)
	{
		param = CreateParameterNamed(name, FSMParameter(value));
	}
	param->AssertParameterFloat();
	param->GetValue().fVal = value;
}

void FSM::SetParameter(const std::string& name, bool value)
{
	auto param = FindParameterNamed(name);
	if (param == nullptr)
	{
		param = CreateParameterNamed(name, FSMParameter(value));
	}
	param->AssertParameterBool();
	param->GetValue().bVal = value;
}

int FSM::GetInt(const std::string& name)
{
	auto param = FindParameterNamed(name);
	assert(param != nullptr && "Couldn't find parameter");
	param->AssertParameterInt();
	return param->GetValue().iVal;
}

float FSM::GetFloat(const std::string& name)
{
	auto param = FindParameterNamed(name);
	assert(param != nullptr && "Couldn't find parameter");
	param->AssertParameterFloat();
	return param->GetValue().fVal;
}

bool FSM::GetBool(const std::string& name)
{
	auto param = FindParameterNamed(name);
	assert(param != nullptr && "Couldn't find parameter");
	param->AssertParameterBool();
	return param->GetValue().bVal;
}

FSMParameter* FSM::FindParameterNamed(std::string name)
{
	MakeStringLowercase(name);
	if (globalParameters.count(name) == 0)
	{
		// Parameter not found string will always be true, and if we are here, we didn't find param
		return nullptr;
	}
	
	auto it = globalParameters.find(name);
	if (it == globalParameters.end())
	{
		return nullptr;
	}

	return &it->second;
}

FSMParameter* FSM::CreateParameterNamed(std::string name, FSMParameter& param)
{
	MakeStringLowercase(name);

	return &(globalParameters.emplace(name, param).first->second);
}

void FSM::Initialise()
{
	currentState_ = defaultState_;
	currentState_.lock()->OnEnterState();
}
