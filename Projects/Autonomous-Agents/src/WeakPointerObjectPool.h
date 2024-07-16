#pragma once

#include "pch.h"
#include "Object.h"
#include "Debug.h"
#include "APoolable.h"
#include "System.h"
#include "Event.h"

template<typename T>
class WeakPointerObjectPool : public Object
{
public:
	WeakPointerObjectPool()
	{
		static_assert((std::is_base_of<APoolable, T>::value, "Type T does not derive from APoolable"));
		eventHandle_levelEnd_ = System::GetInstance()->
				Event_LevelEnd.Subscribe(BindSubscriber(&WeakPointerObjectPool::CleanUpOldObjects, this));
	}

	~WeakPointerObjectPool() override
	{
		CleanUpOldObjects();

		System::GetInstance()->Event_LevelEnd.Unsubscribe(eventHandle_levelEnd_);
	}

	void SetObjectCreationFunction(std::function<std::weak_ptr<T>()> function)
	{
		assert(function != nullptr);
		creationFunction_ = function;
	}

	void AddElements(int amt)
	{
		assert(creationFunction_ != nullptr);
		int i = amt;
		Debug::Print("expanding " + Debug::GetTypename<T>() + " pool, from " +
					 std::to_string(poolSize_) + " to " + std::to_string(poolSize_ + amt));
		while (i--)
		{
			pool_.push_back(creationFunction_());
		}
		poolSize_ += amt;
		availableCount_ += amt;
	}

	std::weak_ptr<T> GetFromPool()
	{
		if (availableCount_ == 0)
		{
			AddElements(30);
		}
		typename std::vector<std::weak_ptr<T>>::iterator it = std::find_if(pool_.begin(),
																		   pool_.end(),
																		   [](std::weak_ptr<T> item) -> bool
																			   {
																				   if (auto obj = std::dynamic_pointer_cast<APoolable>(item.lock()))
																				   {
																					   return !obj->bInUse_;
																				   }
																				   return false;
																			   });

		auto obj = std::dynamic_pointer_cast<APoolable>((*it).lock());
		obj->InitPoolable();
		availableCount_--;
		return *it;		
	}

	void ReturnToPool(std::weak_ptr<T> object)
	{
		if(auto poolable = std::dynamic_pointer_cast<APoolable>(object.lock()))
		{
			auto it = std::find_if(pool_.begin(),
								pool_.end(),
								[&poolable](std::weak_ptr<T>& obj) -> bool
								{
									return std::dynamic_pointer_cast<APoolable>(obj.lock()) == poolable;
								});
			if (it != pool_.end())
			{
				std::dynamic_pointer_cast<APoolable>((*it).lock())->SetIsInUse(false);
				availableCount_++;
			}
		}
	}

	int GetPoolSize() const { return poolSize_; }

	void CleanUpOldObjects()
	{
		pool_.clear();
		poolSize_ = availableCount_ = 0;
	}

private:
	std::vector<std::weak_ptr<T>> pool_;
	std::function<std::weak_ptr<T>()> creationFunction_;

	int poolSize_ = 0;
	int availableCount_ = 0;

	uint32_t eventHandle_levelEnd_ = 0;
};
