#include "pch.h"
#include "FSMParameter.h"


FSMParameter::FSMParameter(int val) :
	type_(ParamType::VT_int)
{
	value_.iVal = val;
}

FSMParameter::FSMParameter(float val) :
	type_(FSMParameter::ParamType::VT_float)
{
	value_.fVal = val;
}

FSMParameter::FSMParameter(bool val) :
	type_(FSMParameter::ParamType::VT_bool)
{
	value_.bVal = val;
}

bool FSMParameter::operator<(const FSMParameter& lhs) const
{
	if (type_ == ParamType::VT_bool)
	{
		return false;
	}

	switch (type_)
	{
	case ParamType::VT_int:
		return lhs.value_.iVal < value_.iVal;
	case ParamType::VT_float:
		return lhs.value_.fVal < value_.fVal ;
	case ParamType::VT_bool:
	default:
		return false;
	}
}

bool FSMParameter::operator>(const FSMParameter& lhs) const
{
	if (type_ == ParamType::VT_bool)
	{
		return false;
	}

	switch (type_)
	{
	case ParamType::VT_int:
		return lhs.value_.iVal > value_.iVal;
	case ParamType::VT_float:
		return lhs.value_.fVal > value_.fVal;
	case ParamType::VT_bool:
	default:
		return false;
	}
}

bool FSMParameter::operator>=(const FSMParameter& lhs) const
{
	switch (type_)
	{
	case ParamType::VT_int:
		return lhs.value_.iVal >= value_.iVal;
	case ParamType::VT_float:
		return lhs.value_.fVal >=value_.fVal;
	case ParamType::VT_bool:
		return lhs.value_.bVal == value_.bVal;
	default:
		return false;
	}
}

bool FSMParameter::operator<=(const FSMParameter& lhs) const
{
	switch (type_)
	{
	case ParamType::VT_int:
		return value_.iVal <= lhs.value_.iVal;
	case ParamType::VT_float:
		return value_.fVal <= lhs.value_.fVal;
	case ParamType::VT_bool:
		return lhs.value_.bVal == value_.bVal;
	default:
		return false;
	}
}

bool FSMParameter::operator==(const FSMParameter& lhs) const
{
	switch (type_)
	{
	case ParamType::VT_int:
		return lhs.value_.iVal == value_.iVal;
	case ParamType::VT_float:
		return lhs.value_.fVal == value_.fVal;
	case ParamType::VT_bool:
		return lhs.value_.bVal == value_.bVal;
	default:
		return false;
	}
}

bool FSMParameter::HasSameType(const FSMParameter* lhs) const
{
	return GetType() == lhs->GetType();
}
