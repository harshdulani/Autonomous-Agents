#include "InputComponent.h"
#include "System.h"

InputComponent::~InputComponent()
{
	UnmapAllControlBindings();
}

void InputComponent::MapControlBinding(sf::Keyboard::Scancode scancode,
									   InputHandler::EButtonState buttonState,
									   const std::function<void()>& callback)
{
	ControlBindingData data;
	data.id = System::GetInstance()->GetInputHandler().AddControlBinding(scancode, buttonState, callback);
	data.scancode = scancode;
	data.buttonState = buttonState;
	bindings_.push_back(data);
}

void InputComponent::MapControlBinding(sf::Mouse::Button mouseButton,
								   InputHandler::EButtonState buttonState,
								   const std::function<void(int, int)>& callback)
{
	ControlBindingData data;
	data.id = System::GetInstance()->GetInputHandler().AddControlBinding(mouseButton, buttonState, callback);
	data.mouseButton = mouseButton;
	data.bMouse = true;
	data.buttonState = buttonState;
	bindings_.push_back(data);
}

void InputComponent::UnmapControlBinding(sf::Keyboard::Scancode scancode, InputHandler::EButtonState buttonState)
{
	for (auto it = bindings_.begin(); it != bindings_.end(); ++it)
	{
		if (!it->bMouse && it->scancode == scancode && it->buttonState == buttonState)
		{
			System::GetInstance()->GetInputHandler().RemoveControlBinding(scancode, buttonState, it->id);
			bindings_.erase(it);
			return;
		}
	}
}

void InputComponent::UnmapControlBinding(sf::Mouse::Button mouseButton, InputHandler::EButtonState buttonState)
{
	for (auto it = bindings_.begin(); it != bindings_.end(); ++it)
	{
		if (it->bMouse && it->mouseButton == mouseButton && it->buttonState == buttonState)
		{
			System::GetInstance()->GetInputHandler().RemoveControlBinding(mouseButton, buttonState, it->id);
			bindings_.erase(it);
			return;
		}
	}
}

void InputComponent::UnmapAllControlBindings()
{
	auto& input = System::GetInstance()->GetInputHandler();
	for (const auto& binding : bindings_)
	{
		input.RemoveControlBinding(binding.scancode, binding.buttonState, binding.id);
	}
	bindings_.clear();
}
