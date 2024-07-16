#include "pch.h"
#include "TransitionEdge.h"
#include "StateNode.h"
#include "FSMParameter.h"

TransitionEdgeCondition::TransitionEdgeCondition(std::string name,
												 TransitionEdgeComparator desiredComparator,
												 FSMParameter desiredCompareParameter) :
	comparisonParameter_(FSMParameter(desiredCompareParameter)),
	comparisonName_(std::move(name)),
	comparator_(desiredComparator)
{
}

bool TransitionEdgeCondition::DoesConditionMatch(const FSMParameter* lhs) const
{
	//"this" is rhs
	if(!comparisonParameter_.HasSameType(lhs))
	{
		return false;
	}

	switch (comparator_)
	{
		case TransitionEdgeComparator::Equals:
			return *lhs == comparisonParameter_;

		case TransitionEdgeComparator::GreaterThan:
			return *lhs > comparisonParameter_;
		case TransitionEdgeComparator::GreaterEquals:
			return *lhs >= comparisonParameter_;

		case TransitionEdgeComparator::LesserThan:
			return *lhs < comparisonParameter_;
		case TransitionEdgeComparator::LesserEquals:
			return *lhs <= comparisonParameter_;

		default:
			return false;
	}
}

void TransitionEdge::AddTransitionCondition(const std::string& paramName,
	TransitionEdgeComparator desiredComparator,
	FSMParameter desiredCompareParam)
{
	transitionConditions_.emplace_back(paramName, desiredComparator, desiredCompareParam);
}

std::vector<TransitionEdgeCondition>& TransitionEdge::GetTransitionConditions()
{
	return transitionConditions_;
}
