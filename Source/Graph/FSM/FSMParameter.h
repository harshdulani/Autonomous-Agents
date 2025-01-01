#pragma once
#include <cassert>

class FSMParameter
{
public:
	FSMParameter(int val);
	FSMParameter(float val);
	FSMParameter(bool val);

	bool HasSameType(const FSMParameter* lhs) const;
	void AssertParameterInt() const { assert(type_ == ParamType::VT_int && "Parameter types don't match"); }
	void AssertParameterFloat() const { assert(type_ == ParamType::VT_float && "Parameter types don't match"); }
	void AssertParameterBool() const { assert(type_ == ParamType::VT_bool && "Parameter types don't match"); }

	union ParamValue
	{
		int iVal;
		float fVal;
		bool bVal;
	};

	bool operator==(const FSMParameter& lhs) const;
	bool operator<(const FSMParameter& lhs) const;
	bool operator>(const FSMParameter& lhs) const;
	bool operator<=(const FSMParameter& lhs) const;
	bool operator>=(const FSMParameter& lhs) const;

	enum class ParamType
	{
		VT_int, VT_float, VT_bool
	};

	const ParamType& GetType() const { return type_; }
	ParamValue& GetValue() { return value_; }

private:
	FSMParameter() = delete;

	ParamValue value_;
	const ParamType type_;
};