#include "APoolable.h"

bool APoolable::operator==(const APoolable& rhs)
{
	return this == &rhs;
}

bool APoolable::GetIsInUse() const
{
	return isInUse;
}

void APoolable::SetIsInUse(bool status)
{
	isInUse = status;
}
