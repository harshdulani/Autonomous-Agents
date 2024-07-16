#pragma once
#include "pch.h"

class Debug
{
public:
	static void Print(const std::string& msg);
	static void Print(const sf::Vector2f& Vec);
	static std::string GetString(const sf::Vector2f& Vec);
	static std::string GetString(const sf::Color& Col);
	
	static void PrintNumberAsBitset(int32_t num);
	static std::string GetNumberAsBitset(int32_t num);

	template<typename T>
	static std::string GetTypename(T object)
	{
		return typeid(object).name();
	}
	
	template<typename T>
	static std::string GetTypename()
	{
		return typeid(T).name();
	}
};
