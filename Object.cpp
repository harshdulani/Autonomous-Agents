#include "Object.h"

void Object::Kill()
{
	bPendingKill = true;
}

bool Object::IsPendingKill() const { return bPendingKill; }

// check memory addresses to confirm if they are the same instance or not
bool Object::operator==(const Object& rhs) const
{
	return this == &rhs;
}

void Object::SetObjectIndex(int Index)
{
	ObjectIndex = Index;
}

int Object::GetObjectIndex() const { return ObjectIndex;}