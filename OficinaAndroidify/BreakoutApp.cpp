#include "BreakoutApp.h"
#include <OficinaFramework/InputSystem.hpp>
#include <ostream>
#include <iomanip>
#include <algorithm>
using namespace OficinaFramework;

#include "MainMenuApp.h"

/// CAMPOS GERAIS
int         LEVEL         = 0;
bool        RUMBLE        = false;
int         LIVES         = 5;
std::string LEVELNAME     = "Unknown";
int         SCORE         = 0;
int         HIGHSCORE     = 0;
int         MULTIPLIER    = 0;
bool        SHOWTUTORIAL  = true;
bool        RESET_GAME    = true;
bool        PAUSED        = false;

#include "Block.h"
#include "Ball.h"
#include "Pad.h"
#include "LevelRoutines.h"

Pad* p;
Ball* b;
std::list<Block*> blocks;
float oldMousePos, mousePos;

void BreakoutApp::setup()
{
	size(1280, 720);
	frameRate(60);

	p = new Pad;
	b = new Ball;

	p->TrackInput();
	b->TrackPad(p);
	b->TrackBlocks(&blocks);
	BuildLevel(levels[LEVEL]);
	noStroke();
	oldMousePos = mousePos = 360.0f;
}

void checkResetGame()
{
	if (!RESET_GAME) return;
	LEVEL = -1;
	if (SCORE > HIGHSCORE) HIGHSCORE = SCORE;
	SCORE = 0;
	LIVES = 5;
	for (Block* b : blocks)
		delete b;
	blocks.clear();
	SHOWTUTORIAL = true;
	RUMBLE       = false;
	RESET_GAME   = false;
}

void displayHUD()
{
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float height = static_cast<float>(RenderingSystem::GetResolution().y);

	// Continues
	float ballDiameter = height * 0.034723f;
	pushMatrix();
	fill(255);
	translate(width - ballDiameter, height - ballDiameter);
	for (int i = 0; i < LIVES; i++) {
		ellipse(0, 0, ballDiameter / 2.0f, ballDiameter / 2.0f);
		translate(-ballDiameter - 5.0f, 0.0f);
	}
	popMatrix();

	// Level name
	textSize(16.0f);
	textAlign(LEFT, CENTER);

	std::stringstream oss;
	oss.str("");
	oss << "LEVEL " << (RUMBLE ? 10 + LEVEL : LEVEL) << ": " + LEVELNAME;
	text(oss.str(), ballDiameter, height - ballDiameter);
	textAlign(CENTER, CENTER);

	oss.str("");
	oss << std::setfill('0') << std::setw(10) << SCORE;

	text(oss.str(), width / 2.0f, height - ballDiameter);

	if (MULTIPLIER >= 2)
	{
		oss.str("");
		oss << "Chain Bonus x" << MULTIPLIER;
		text(oss.str(), 3.0f * width / 4.0f, height - ballDiameter);
	}


	// Tutorial, high score, etc
	if (SHOWTUTORIAL) {
		fill(0, 110, 0);
		textSize(16.0f);
		textAlign(CENTER, CENTER);

		oss.str("");
		oss << "HIGH SCORE: " << std::setfill('0') << std::setw(10) << HIGHSCORE;

		text(oss.str(), width / 2.0f, height / 4.0f);
		fill(255);
		textSize(32.0f);
		text("SUPER BRICKBREAK", width / 2.0f, height / 2.0f);
		textSize(16.0f);

		oss.str("");
		if (InputSystem::GetType() == InputSystem::Type::KEYBOARD)
			 oss << "       LEFT/RIGHT, MOUSE = MOVE PAD                \n"
			        "    S, LEFT MOUSE BUTTON = RELEASE BALL            \n"
			        "                   ENTER = PAUSE                   \n"
			        "                       A = MOVE FASTER             \n"
			        "               BACKSPACE = RETURN TO MAIN MENU     ";
		else oss << "              LEFT STICK = MOVE PAD                \n"
			        "                       A = RELEASE BALL            \n"
			        "                   START = PAUSE                   \n"
					"                      RT = MOVE FASTER             \n"
			        "                    BACK = RETURN TO MAIN MENU     ";
		text(oss.str(), width / 2.0f, 3.0f*height / 4.0f);
	}

	if (PAUSED) {
		textSize(32.0f);
		textAlign(CENTER, CENTER);
		text("PAUSE", width / 2.0f, height / 2.0f);
	}

}

void BreakoutApp::unload()
{
	delete p;
	delete b;
	for (auto b : blocks)
		delete b;
	blocks.clear();

	LEVEL = 0;
	RUMBLE = false;
	LIVES = 5;
	LEVELNAME = "Unknown";
	SCORE = 0;
	MULTIPLIER = 0;
	SHOWTUTORIAL = true;
	RESET_GAME = true;
	PAUSED = false;
}

void BreakoutApp::updateInput()
{
	// Mouse controls
	if (InputSystem::GetType() == InputSystem::Type::KEYBOARD)
	{
		oldMousePos = mousePos;
		mousePos = static_cast<float>(InputSystem::GetMousePos().x);
		mousePos /= static_cast<float>(RenderingSystem::GetViewportSize().x);
		mousePos *= static_cast<float>(RenderingSystem::GetResolution().x);
		mousePos += RenderingSystem::GetViewportPosition().x;
		if (oldMousePos != mousePos && !PAUSED)
		{
			p->setPosition(mousePos);
		}
	}

	if (InputSystem::PressedKey(SDL_SCANCODE_F1))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());

	// Fullscreen
	if ((InputSystem::PressingKey(SDL_SCANCODE_LALT) || InputSystem::PressingKey(SDL_SCANCODE_RALT))
		&& InputSystem::PressedKey(SDL_SCANCODE_RETURN))
	{
		ScreenSystem::SetFullScreen(!ScreenSystem::IsFullScreen());
	}
	// Wake up
	if ((InputSystem::PressedButton(InputSystem::GamePadButton::A)
		|| InputSystem::PressedMouse(InputSystem::MouseButton::LEFTMB))
		&& !PAUSED)
	{
		b->wakeUp();
		SHOWTUTORIAL = false;
	}

	// Pause
	if (InputSystem::PressedButton(InputSystem::GamePadButton::START)
		&& !SHOWTUTORIAL)
	{
		PAUSED = !PAUSED;
	}

	// Menu
	if (InputSystem::PressedButton(InputSystem::GamePadButton::SELECT))
	{
		ScreenSystem::AddScreen(new MainMenuApp);
		RemoveMe();
	}

	// Hide mouse when not paused
	if (PAUSED) InputSystem::ShowMouse();
	else        InputSystem::HideMouse();

	// Debug
	if (InputSystem::PressedKey(SDL_SCANCODE_Q))
		LIVES = 5;
	if (InputSystem::PressedKey(SDL_SCANCODE_W))
	{
		for (Block* b : blocks)
			delete b;
		blocks.clear();
		SCORE -= 500;
	}
}

void BreakoutApp::draw()
{
	updateInput();

	background(0);
	if (!PAUSED) {
		p->update();
		b->update();
	}
	p->draw();
	b->draw();
	for (Block* b : blocks)
	{
		if(!PAUSED)
			b->update();
		b->draw();
	}

	// if the level is cleared, move up to the next!;
	if (blocks.size() == 0) {
		if (LEVEL < (RUMBLE ? 10 : 9))
			LEVEL++;
		else {
			LEVEL = 0;
			RUMBLE = !RUMBLE;
		}

		switch (LEVEL) {
		case 0: LEVELNAME = "Starter";             break;
		case 1: LEVELNAME = "Three Ways";          break;
		case 2: LEVELNAME = "Diamond";             break;
		case 3: LEVELNAME = "Smiley";              break;
		case 4: LEVELNAME = "Stickmen";            break;
		case 5: LEVELNAME = "Stairway to Heaven";  break;
		case 6: LEVELNAME = "Fortress";            break;
		case 7: LEVELNAME = "Bow & Arrow";         break;
		case 8: LEVELNAME = "Full House";          break;
		case 9: LEVELNAME = "This is Not the End"; break;
		}

		if (RUMBLE) {
			if (LEVEL == 9) LEVELNAME = "Thanks for Playing";
			else if (LEVEL == 10) LEVELNAME = "Bonus Stage";
			else LEVELNAME = "Mirror World " + LEVELNAME;
		}

		if (LEVEL != 0) {
			if (!RUMBLE) SCORE += 500;
			else        SCORE += 1000;
		}

		b->respawn();
		if(LEVEL != 10)
			BuildLevel(levels[LEVEL]);
		else
		{
			std::vector<int> randomLevel;
			int amount = random(10, 176);
			while (amount >= 0)
			{
				int randomBlock;
				do { randomBlock = random(176); }
				while (std::find(randomLevel.begin(), randomLevel.end(), randomBlock) != randomLevel.end());
				randomLevel.push_back(randomBlock);
				amount--;
			}
			BuildLevel(randomLevel);
		}
	}

	// Check if we need to reset the game.
	checkResetGame();

	displayHUD();
}
