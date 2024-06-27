#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

class Debug
{
public:
	static void Print(const std::string& msg);
	static void Print(const sf::Vector2f& Vec);
	static std::string GetString(const sf::Vector2f& Vec);
	
	static void PrintNumberAsBitset(int32_t num);

	template<typename T>
	static const std::string& GetTypename(T object)
	{
		return typeid(object).name();
	}
	
	template<typename T>
	static const std::string& GetTypename()
	{
		return typeid(T).name();
	}
};
