#pragma once
#include "pch.h"
#include "../Component.h"
#include "InputHandler.h"

class InputComponent : public Component
{
public:
	~InputComponent() override;
	void MapControlBinding(sf::Keyboard::Scancode scancode,
						   InputHandler::EButtonState buttonState,
						   const std::function<void()>& callback);
	void MapControlBinding(sf::Mouse::Button mouseButton,
						   InputHandler::EButtonState buttonState,
						   const std::function<void(int, int)>& callback);
	void UnmapControlBinding(sf::Keyboard::Scancode scancode,
							 InputHandler::EButtonState buttonState);
	void UnmapControlBinding(sf::Mouse::Button mouseButton,
							 InputHandler::EButtonState buttonState);

	void UnmapAllControlBindings();

private:
	struct ControlBindingData
	{
		uint32_t id;
		sf::Keyboard::Scancode scancode;
		sf::Mouse::Button mouseButton;
		bool bMouse = false;
		InputHandler::EButtonState buttonState;
	};
	std::vector<ControlBindingData> bindings_;
};
