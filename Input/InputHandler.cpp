#include "InputHandler.h"
#include <cassert>
#include <SFML/Window/Event.hpp>
#include "../System.h"

InputHandler::~InputHandler()
{
	UnsubscribeAll();
}

void InputHandler::PollEvents(sf::RenderWindow& window)
{
	if (!window.hasFocus())
		return;
	
	sf::Event currentEvent;
	while (window.pollEvent(currentEvent))
	{
		CheckSystemEvents(currentEvent);
		switch (currentEvent.type)
		{
			case sf::Event::KeyPressed:
			{
				TryInvokeKeyEvent(keyPressSubs_, currentEvent.key.scancode);
				auto it = keysHeld_.begin();
				for (; it != keysHeld_.end(); ++it)
				{
					if (*it == currentEvent.key.scancode)
					{
						break;
					}
				}
				if (it == keysHeld_.end())
				{
					keysHeld_.push_back(currentEvent.key.scancode);
				}
			}
			break;
			case sf::Event::KeyReleased:
			{
				TryInvokeKeyEvent(keyReleasedSubs_, currentEvent.key.scancode);
				for (auto it = keysHeld_.begin(); it != keysHeld_.end(); ++it)
				{
					if (*it == currentEvent.key.scancode)
					{
						keysHeld_.erase(it);
						return;
					}
				}
			}
			break;
			case sf::Event::MouseButtonPressed:
			{
				sf::Event::MouseButtonEvent& mouseEvent = currentEvent.mouseButton;
				TryInvokeKeyEvent(mousePressSubs_, mouseEvent.button, mouseEvent.x, mouseEvent.y);
				bool bFound = false;
				for (auto& mbHeld : mouseButtonsHeld_)
				{
					if (mbHeld.button == mouseEvent.button)
					{
						bFound = true;
						mbHeld.x = mouseEvent.x;
						mbHeld.y = mouseEvent.y;
					}
				}
				if (!bFound)
					mouseButtonsHeld_.push_back(mouseEvent);
			}
			break;
			case sf::Event::MouseButtonReleased:
			{
				sf::Event::MouseButtonEvent& mouseEvent = currentEvent.mouseButton;
				TryInvokeKeyEvent(mouseReleasedSubs_, mouseEvent.button, mouseEvent.x, mouseEvent.y);
				for (auto it = mouseButtonsHeld_.begin(); it != mouseButtonsHeld_.end(); ++it)
				{
					if (mouseEvent.button == it->button)
					{
						mouseButtonsHeld_.erase(it);
						break;
					}
				}
			}
			break;
		}
	}
	for (auto keyHeld : keysHeld_)
	{
		for (auto sub : keyHeldSubs_)
		{
			if (keyHeld == sub.first)
			{
				sub.second.Invoke();
				break;
			}
		}
	}
	for (auto keyHeld : mouseButtonsHeld_)
	{
		for (auto sub : mouseHeldSubs_)
		{
			if (keyHeld.button == sub.first)
			{
				sub.second.Invoke(keyHeld.x, keyHeld.y);
				break;
			}
		}
	}
}

void InputHandler::CheckSystemEvents(const sf::Event& currentEvent)
{
	switch (currentEvent.type)
	{
		case sf::Event::Closed:
			System::GetInstance()->CloseWindowDeferred();
		break;
		case sf::Event::KeyPressed:
			if (currentEvent.key.code == sf::Keyboard::Escape)
				System::GetInstance()->CloseWindowDeferred();
		break;
		default: ;
	}
}

uint32_t InputHandler::AddControlBinding(sf::Keyboard::Scancode scancode,
												   EButtonState buttonState,
												   const std::function<void()>& callback)
{
	if (buttonState == EButtonState::EBS_None)
		return 0;

	std::map<sf::Keyboard::Scancode, Event<>>::iterator it;
	switch (buttonState)
	{
		case EButtonState::EBS_Pressed:
		{
			it = keyPressSubs_.find(scancode);
			if (it == keyPressSubs_.end())
			{
				auto pairIt = keyPressSubs_.emplace(scancode, Event<>());
				if (pairIt.second)
				{
					it = pairIt.first;
				}
			}
		}
		break;
		case EButtonState::EBS_Held:
		{
			it = keyHeldSubs_.find(scancode);
			if (it == keyHeldSubs_.end())
			{
				auto pairIt = keyHeldSubs_.emplace(scancode, Event<>());
				if (pairIt.second)
				{
					it = pairIt.first;
				}
			}
		}
		break;
		case EButtonState::EBS_Released:
		{
			it = keyReleasedSubs_.find(scancode);
			if (it == keyReleasedSubs_.end())
			{
				auto pairIt = keyReleasedSubs_.emplace(scancode, Event<>());
				if (pairIt.second)
				{
					it = pairIt.first;
				}
			}
		}
		break;
		case EButtonState::EBS_None:
			break;
	}
	
	return it->second.Subscribe(callback);
}

uint32_t InputHandler::AddControlBinding(sf::Mouse::Button mouseButton,
												EButtonState buttonState,
												const std::function<void(int, int)>& callback)
{
	if (buttonState == EButtonState::EBS_None)
		return 0;

	std::map<sf::Mouse::Button, Event<int, int>>::iterator it;
	bool bFoundIterator = false;
	switch (buttonState)
	{
		case EButtonState::EBS_Pressed:
		{
			it = mousePressSubs_.find(mouseButton);
			if (it == mousePressSubs_.end())
			{
				auto pairIt = mousePressSubs_.emplace(mouseButton, Event<int, int>());
				if (pairIt.second)
				{
					it = pairIt.first;
					bFoundIterator = true;
				}
			}
		}
		break;
		case EButtonState::EBS_Held:
		{
			it = mouseHeldSubs_.find(mouseButton);
			if (it == mouseHeldSubs_.end())
			{
				auto pairIt = mouseHeldSubs_.emplace(mouseButton, Event<int, int>());
				if (pairIt.second)
				{
					it = pairIt.first;
					bFoundIterator = true;
				}
			}
		}
		break;
		case EButtonState::EBS_Released:
		{
			it = mouseReleasedSubs_.find(mouseButton);
			if (it == mouseReleasedSubs_.end())
			{
				auto pairIt = mouseReleasedSubs_.emplace(mouseButton, Event<int, int>());
				if (pairIt.second)
				{
					it = pairIt.first;
					bFoundIterator = true;
				}
			}
		}
		break;
		case EButtonState::EBS_None:
			break;
	}

	// couldn't find and/or create iterator
	assert(bFoundIterator);

	return it->second.Subscribe(callback);
}

void InputHandler::RemoveControlBinding(sf::Keyboard::Scancode scancode, EButtonState buttonState, uint32_t id)
{
	switch (buttonState)
	{
		case EButtonState::EBS_None:
			break;
		case EButtonState::EBS_Pressed:
		{
			auto it = keyPressSubs_.find(scancode);
			if (it != keyPressSubs_.end())
			{
				it->second.Unsubscribe(id);
			}
		}
		break;
		case EButtonState::EBS_Held:
		{
			auto it = keyHeldSubs_.find(scancode);
			if (it != keyHeldSubs_.end())
			{
				it->second.Unsubscribe(id);
			}
		}
		break;
		case EButtonState::EBS_Released:
		{
			auto it = keyReleasedSubs_.find(scancode);
			if (it != keyReleasedSubs_.end())
			{
				it->second.Unsubscribe(id);
			}
		}
		break;
	}
}

void InputHandler::RemoveControlBinding(sf::Mouse::Button button, EButtonState buttonState, uint32_t id)
{
	switch (buttonState)
	{
		case EButtonState::EBS_None:
			break;
		case EButtonState::EBS_Pressed:
		{
			auto it = mousePressSubs_.find(button);
			if (it != mousePressSubs_.end())
			{
				it->second.Unsubscribe(id);
			}
		}
		break;
		case EButtonState::EBS_Held:
		{
			auto it = mouseHeldSubs_.find(button);
			if (it != mouseHeldSubs_.end())
			{
				it->second.Unsubscribe(id);
			}
		}
		break;
		case EButtonState::EBS_Released:
		{
			auto it = mouseReleasedSubs_.find(button);
			if (it != mouseReleasedSubs_.end())
			{
				it->second.Unsubscribe(id);
			}
		}
		break;
	}
}

void InputHandler::UnsubscribeAll()
{
	keysHeld_.clear();
	for (auto& sub : keyPressSubs_)
	{
		sub.second.UnsubscribeAll();
	}
	keyPressSubs_.clear();
	for (auto& sub : keyHeldSubs_)
	{
		sub.second.UnsubscribeAll();
	}
	keyHeldSubs_.clear();
	for (auto& sub : keyReleasedSubs_)
	{
		sub.second.UnsubscribeAll();
	}
	keyReleasedSubs_.clear();
	
	mouseButtonsHeld_.clear();
	for (auto& sub : mousePressSubs_)
	{
		sub.second.UnsubscribeAll();
	}
	mousePressSubs_.clear();
	for (auto& sub : mouseHeldSubs_)
	{
		sub.second.UnsubscribeAll();
	}
	mouseHeldSubs_.clear();
	for (auto& sub : mouseReleasedSubs_)
	{
		sub.second.UnsubscribeAll();
	}
	mouseReleasedSubs_.clear();
}
