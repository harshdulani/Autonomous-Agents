#include "pch.h"
#include "APoolable.h"

bool APoolable::operator==(const APoolable& rhs)
{
	return this == &rhs;
}

bool APoolable::GetIsInUse() const
{
	return bInUse_;
}

void APoolable::SetIsInUse(bool status)
{
	bInUse_ = status;
}
