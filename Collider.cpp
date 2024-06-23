#include "Collider.h"

bool Collider::IsValidCollider() const
{
	return IsEnabled() && !IsPendingKill() && Radius > 0.0f;
}
