#pragma once

class APoolable
{
	template<typename T>
	friend class WeakPointerObjectPool;
	template<typename T>
	friend class ObjectPool;
public:
	virtual bool operator==(const APoolable& rhs);

protected:
	APoolable() = default;
	virtual ~APoolable() = default;

	virtual void InitPoolable() = 0;

	bool GetIsInUse() const;
	void SetIsInUse(bool status);

private:
	bool bInUse_ = false;
};
