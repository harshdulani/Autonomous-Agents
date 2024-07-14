#pragma once

#include <functional>
#include <map>

template<typename... Args>
class Event
{
public:
	void Invoke(Args... args) const
	{
		if (subscribers_.empty())
		{
			return;
		}
		for (auto& subscriber : subscribers_)
		{
			subscriber.second(args...);
		}
	}

	uint32_t Subscribe(std::function<void(Args...)> callback)
	{
		int newId = global_event_id_++;
		subscribers_.emplace(newId, callback);
		return newId;
	}

	void Unsubscribe(uint32_t id)
	{
		if (subscribers_.count(id) == 0)
		{
			return;
		}

		subscribers_.erase(id);
	}

	void UnsubscribeAll()
	{
		subscribers_.clear();
	}

private:
	std::map<int, std::function<void(Args...)>> subscribers_;

	static uint32_t global_event_id_;
};

template<typename... Args>
uint32_t Event<Args...>::global_event_id_ = 1;

/// <summary>
/// Returns a (lambda) function object for you to bind to an event
/// </summary>
/// <typeparam name="ClassObject">"this" or "&obj"</typeparam>
/// <typeparam name="Class">The owning class of the member function</typeparam>
/// <typeparam name="ReturnType">The return type of the subscribing function (must be same as event signature)</typeparam>
/// <typeparam name="...Args">To be used during function call</typeparam>
/// <param name="MemberFunction">FunctionName</param>
/// <returns></returns>
template<typename ReturnType, typename Class, typename ClassObject, typename... Args>
std::function<ReturnType(Args...)> BindSubscriber(ReturnType(Class::* MemberFunction)(Args...), ClassObject instance)
{
	return [instance, MemberFunction](Args... args) -> ReturnType
		{
			return (instance->*MemberFunction)(args...);
		};
};
