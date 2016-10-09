#pragma once
#include <OficinaFramework\OficinaTypes.hpp>
#include <OficinaFramework\InputSystem.hpp>
#include "util.h"
using namespace OficinaFramework;

class Pad
{
public:
	vec2 position;
	vec2 size;
	color c;
	float xspeed;
	bool tracking = false;
	float maxspeed;
	float truemaxspeed;

	// Boundary Aliases
	float topedge,
		bottomedge,
		leftedge,
		rightedge;

	Pad();
	void TrackInput();
	void UntrackInput();
	void setPosition(float x);
	void update();
	void draw();
};