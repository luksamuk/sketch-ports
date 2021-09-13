#include "AndroidifyApp.h"
#include "AndroidRobot.h"
#include "Panel.h"
#include <OficinaFramework/InputSystem.hpp>
using namespace OficinaFramework;

#include "MainMenuApp.h"

AndroidRobot* robot;
Panel* panel;
bool mouseClick = false;

bool BACKTOMENU = false;

void AndroidifyApp::setup()
{
	size(1280, 720);
	robot = new AndroidRobot(vec2(width / 2.0f, (height / 2.0f) + 50.0f));
	panel = new Panel(robot, width, height);
}

void AndroidifyApp::unload()
{
	delete panel;
	delete robot;
}

void AndroidifyApp::draw()
{
	background(0);

	robot->setPosition(vec2((panel->getPosition().x + (width / 2.0f) + width) / 2.0f,
							(height / 2.0f) + 50.0f));

	robot->update();
	robot->draw();
	panel->update(width, height);
	stroke(0);
	panel->draw();

	if (InputSystem::PressedKey(SDL_SCANCODE_F1))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());

	if (BACKTOMENU)
	{
		BACKTOMENU = false;
		ScreenSystem::AddScreen(new MainMenuApp);
		RemoveMe();
	}

	// Fullscreen
	if ((InputSystem::PressingKey(SDL_SCANCODE_LALT) || InputSystem::PressingKey(SDL_SCANCODE_RALT))
		&& InputSystem::PressedKey(SDL_SCANCODE_RETURN))
	{
		ScreenSystem::SetFullScreen(!ScreenSystem::IsFullScreen());
	}

	vec2 mousePos = InputSystem::GetMousePos().toVec2();
	mousePos /= RenderingSystem::GetViewportSize().toVec2();
	mousePos *= RenderingSystem::GetResolution().toVec2();
	mousePos += RenderingSystem::GetViewportPosition();

	// Clicking
	if (InputSystem::PressingMouse(InputSystem::MouseButton::LEFTMB) && !mouseClick) {
		mouseClick = true;
		panel->evalClick(mousePos);
	}
	// Unclicked
	if (!InputSystem::PressingMouse(InputSystem::MouseButton::LEFTMB) && mouseClick) {
		mouseClick = false;
		panel->evalUnclick(mousePos);
	}

}
