#pragma once

class Object
{
	friend class ObjectManager;
public:
	Object() = default;
	virtual ~Object() = default;
	virtual void Kill();

	bool IsDeleteDeferred() const;
	bool operator==(const Object& rhs) const;
	
private:
	int GetObjectIndex() const;
	void SetObjectIndex(int Index);
	bool bDeleteDeferred = false;

	int ObjectIndex = -1;
};
