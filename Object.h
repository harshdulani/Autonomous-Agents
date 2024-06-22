#pragma once

class Object
{
	friend class ObjectManager;
public:
	Object() = default;
	virtual ~Object() = default;
	virtual void Kill();

	bool IsPendingKill() const;
	bool operator==(const Object& rhs) const;
	
private:
	int GetObjectIndex() const;
	void SetObjectIndex(int Index);
	bool bPendingKill = false;

	int ObjectIndex = -1;
};
