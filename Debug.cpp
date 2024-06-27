#include "Debug.h"
#include <bitset>
#include <iostream>

void Debug::Print(const std::string& msg)
{
	std::cout << msg << "\n";
}

void Debug::Print(const sf::Vector2f& Vec)
{
	std::cout << "(" << Vec.x << ", " << Vec.y << ")\n";
}

std::string Debug::GetString(const sf::Vector2f& Vec)
{
	std::string out;
	out += "(";
	out += std::to_string(Vec.x);
	out += ", ";
	out += std::to_string(Vec.y);
	out += ")\n";
	return out;	
}

void Debug::PrintNumberAsBitset(int32_t num)
{
	std::cout << std::bitset<32>(num).to_string() << "\n";
}
