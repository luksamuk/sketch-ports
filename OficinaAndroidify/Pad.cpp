#include "Pad.h"
#include <OficinaFramework\RenderingSystem.hpp>
#include <OficinaFramework\InputSystem.hpp>
#include <OficinaFramework\TimingSystem.hpp>

Pad::Pad()
{
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float height = static_cast<float>(RenderingSystem::GetResolution().y);

	position = vec2(width / 2.0f, 11 * height / 12.0f);
	size = vec2(width*0.12f, height*0.034723f);
	// 75% of ball's speed for balanced difficulty
	truemaxspeed = height / 72.0f * 0.75; // Which gives us 75% of the ball speed
	c = color(255);
}

void Pad::TrackInput()
{
	tracking = true;
}

void Pad::UntrackInput() {
	tracking = false;
}

void Pad::setPosition(float x) {
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	position.x = x;
	clamp(position.x, size.x / 2.0f, width - (size.x / 2.0f));
}

void Pad::update() {
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float frameRate = static_cast<float>(TimingSystem::GetFPS());
	if (tracking) {
		float lstick = InputSystem::GetLeftStick().x;
		maxspeed = truemaxspeed + (truemaxspeed * InputSystem::GetTrigger(InputSystem::GamePadTrigger::RIGHTTRIGGER));
		position.x += maxspeed * lstick * 60.0f / frameRate;
	}
	clamp(position.x, size.x / 2.0f, width - (size.x / 2.0f));

	// Aliases for boundaries
	topedge = position.y - (size.y / 2.0f);
	bottomedge = position.y + (size.y / 2.0f);
	leftedge = position.x - (size.x / 2.0f);
	rightedge = position.x + (size.x / 2.0f);
}

void Pad::draw()
{
	pushMatrix();
	translate(position.x, position.y);
	fill(c);
	rect(-size.x / 2.0f, -size.y / 2.0f, size.x, size.y);
	popMatrix();
}
