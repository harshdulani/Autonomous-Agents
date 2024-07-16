#pragma once


#include "pch.h"
#include "Edge.h"
#include "FSMParameter.h"

class StateNode;

enum class TransitionEdgeComparator;
class TransitionEdgeCondition;

class TransitionEdge : public Edge
{
public:
	void AddTransitionCondition(const std::string& paramName, 
		TransitionEdgeComparator desiredComparator, 
		FSMParameter desiredCompareParam);

	std::vector<TransitionEdgeCondition>& GetTransitionConditions();

private:
	std::vector<TransitionEdgeCondition> transitionConditions_;
};

enum class TransitionEdgeComparator
{
	Equals,
	GreaterThan, GreaterEquals,
	LesserThan, LesserEquals
};
class TransitionEdgeCondition
{
public:
	TransitionEdgeCondition(std::string name,
							TransitionEdgeComparator desiredComparator,
							FSMParameter desiredCompareParameter);

	bool DoesConditionMatch(const FSMParameter* lhs) const;
	inline const std::string& GetConditionParamName() { return comparisonName_; }

private:
	// this will store the desired value, type for the parameter of desired name
	FSMParameter comparisonParameter_;
	//desiredName
	const std::string comparisonName_;

	// to be used by comparator function
	TransitionEdgeComparator comparator_;
};
