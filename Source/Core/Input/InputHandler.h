#pragma once
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../Event.h"

class InputHandler
{
public:
	~InputHandler();
	
	void PollEvents(sf::RenderWindow& window);

	enum class EButtonState { EBS_None, EBS_Pressed, EBS_Held, EBS_Released };

	uint32_t AddControlBinding(sf::Keyboard::Scancode scancode,
							   EButtonState buttonState,
							   const std::function<void()>& callback);

	uint32_t AddControlBinding(sf::Mouse::Button mouseButton,
							   EButtonState buttonState,
							   const std::function<void(int, int)>& callback);

	void RemoveControlBinding(sf::Keyboard::Scancode scancode,
							  EButtonState buttonState,
							  uint32_t id);

	void RemoveControlBinding(sf::Mouse::Button button,
							  EButtonState buttonState,
							  uint32_t id);
	
private:
	template<typename T, typename... Args>
	void TryInvokeKeyEvent(const std::map<T, Event<Args...>>& subscriberMap, T button, Args... args);

	void UnsubscribeAll();

	static void CheckSystemEvents(const sf::Event& currentEvent);

private:
	std::vector<sf::Keyboard::Scancode> keysHeld_;
	// keyboard event subscribers by key
	std::map<sf::Keyboard::Scancode, Event<>> keyPressSubs_;
	std::map<sf::Keyboard::Scancode, Event<>> keyHeldSubs_;
	std::map<sf::Keyboard::Scancode, Event<>> keyReleasedSubs_;

	std::vector<sf::Event::MouseButtonEvent> mouseButtonsHeld_;
	// mouse event subscribers by button
	std::map<sf::Mouse::Button, Event<int, int>> mousePressSubs_;
	std::map<sf::Mouse::Button, Event<int, int>> mouseHeldSubs_;
	std::map<sf::Mouse::Button, Event<int, int>> mouseReleasedSubs_;
};

template<typename T, typename... Args>
void InputHandler::TryInvokeKeyEvent(const std::map<T, Event<Args...>>& subscriberMap, T button, Args... args)
{
	auto sub = subscriberMap.find(button);
	if (sub != subscriberMap.end())
		sub->second.Invoke(args...);
}