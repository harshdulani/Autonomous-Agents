#include "Object.h"

void Object::Kill()
{
	bDeleteDeferred = true;
}

bool Object::IsDeleteDeferred() const { return bDeleteDeferred; }

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