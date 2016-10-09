#include "MainMenuApp.h"
#include <OficinaFramework\InputSystem.hpp>
#include <OficinaFramework\RenderingSystem.hpp>
using namespace OficinaFramework;

#include "AndroidifyApp.h"
#include "BreakoutApp.h"

const std::vector<std::string> menuOptions =
{
	"Androidify",
	"Super Brickbreak",
	"Snake",
	"Exit"
};
int option = 0;

void MainMenuApp::setup()
{
	size(1280, 720);
	InputSystem::HideMouse();
}

void MainMenuApp::unload()
{

}

void MainMenuApp::draw()
{
	float width  = RenderingSystem::GetResolution().x;
	float height = RenderingSystem::GetResolution().y;

	if (InputSystem::MovedStick(
		InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::VERTICAL,
		InputSystem::ThumbStickAxisSignal::POSITIVE))
		option++;
	else if (InputSystem::MovedStick(
		InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::VERTICAL,
		InputSystem::ThumbStickAxisSignal::NEGATIVE))
		option--;

	if (option < 0) option = menuOptions.size() - 1;
	else if (option >= menuOptions.size()) option = 0;

	if (InputSystem::PressedButton(InputSystem::GamePadButton::A))
	{
		switch (option)
		{
		case 0: // Androidify
			ScreenSystem::AddScreen(new AndroidifyApp);
			InputSystem::ShowMouse();
			break;
		case 1: // Breakout
			ScreenSystem::AddScreen(new BreakoutApp);
			break;
		//case 2: // Snake
		//	break;
		default: // Quits
			InputSystem::CallExitCommand();
			break;
		};
		RemoveMe();
	}

	if (InputSystem::PressedKey(SDL_SCANCODE_F1))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());

	// Fullscreen
	if ((InputSystem::PressingKey(SDL_SCANCODE_LALT) || InputSystem::PressingKey(SDL_SCANCODE_RALT))
		&& InputSystem::PressedKey(SDL_SCANCODE_RETURN))
	{
		ScreenSystem::SetFullScreen(!ScreenSystem::IsFullScreen());
	}


	background(0);
	fill(255);
	textSize(25.0f);

	textAlign(CENTER);
	text("Select Sketch", width / 2.0f, 60.0f);
	if(InputSystem::GetType() == InputSystem::Type::KEYBOARD)
		text("Press S to select", width / 2.0f, 85.0f);
	else text("Press A to select", width / 2.0f, 85.0f);

	triangle((width / 3.0f) - 37.5f, 118.75f + 60.0f * option,
		(width / 3.0f) - 25.0f, 137.5f + 60.0f * option,
		(width / 3.0f) - 37.5f, 156.25f + 60.0f * option);
	int drawOption = 0;
	textAlign(LEFT);
	for (auto str : menuOptions)
		text(str, width / 3.0f, 150.0f + 60.0f * (drawOption++));
}