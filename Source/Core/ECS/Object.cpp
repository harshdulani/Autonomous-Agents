#include "Object.h"

void Object::Kill()
{
	bPendingKill_ = true;
}

bool Object::IsPendingKill() const { return bPendingKill_; }

// check memory addresses to confirm if they are the same instance or not
bool Object::operator==(const Object& rhs) const
{
	return this == &rhs;
}

void Object::SetObjectIndex(int Index)
{
	objectIndex_ = Index;
}

int Object::GetObjectIndex() const { return objectIndex_;}