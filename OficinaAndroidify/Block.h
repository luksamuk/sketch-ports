#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include "util.h"
using namespace OficinaFramework;

class Block
{
public:
	vec2 position,
		orig_position;

	vec2 size;

	color c,
		c_mediumtones,
		c_darktones;

	float topedge,
		bottomedge,
		leftedge,
		rightedge;
	float displace_ratio;

	Block(float x, float y);
	void update();
	void draw();
};
