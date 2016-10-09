#include <OficinaFramework/OficinaFramework.hpp>
#include <list>
#include <string>
#include "MainMenuApp.h"
using namespace OficinaFramework;

int main(int argc, char** argv)
{
	// Remember to have folders assets/ and resources/ inside your
	// project. Make sure prepare_assets.bat is inside resources/
	// folder.

	std::list<std::string>* confv = new std::list<std::string>;
	confv->push_back("gamename=Processing (C++/OficinaFramework)");
	confv->push_back("framerate=60hz");
	confv->push_back("resolution=720p");
	//confv->push_back("iconpath=icon");
	

	EngineCore::Initialize(argc, argv, confv);
	RenderingSystem::SetLinearFiltering(false);
	ScreenSystem::Debug_SetFont(RenderingSystem::TexturePool::LoadDefaultFont());
	
	InputSystem::BindStick(SDLK_LEFT,
		InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::HORIZONTAL,
		InputSystem::ThumbStickAxisSignal::NEGATIVE);
	InputSystem::BindStick(SDLK_RIGHT,
		InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::HORIZONTAL,
		InputSystem::ThumbStickAxisSignal::POSITIVE);
	InputSystem::BindStick(SDLK_UP,
		InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::VERTICAL,
		InputSystem::ThumbStickAxisSignal::NEGATIVE);
	InputSystem::BindStick(SDLK_DOWN,
		InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::VERTICAL,
		InputSystem::ThumbStickAxisSignal::POSITIVE);
	InputSystem::BindKey(SDLK_a, InputSystem::GamePadButton::RSHOULDER2);
	InputSystem::BindKey(SDLK_s, InputSystem::GamePadButton::A);
	InputSystem::BindKey(SDLK_RETURN, InputSystem::GamePadButton::START);
	InputSystem::BindKey(SDLK_BACKSPACE, InputSystem::GamePadButton::SELECT);

	ScreenSystem::AddScreen(new MainMenuApp);


	return EngineCore::DoGameLoop();
}