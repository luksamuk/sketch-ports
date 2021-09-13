#include <OficinaFramework/RenderingSystem.hpp>
#include "Block.h"

extern bool RUMBLE;

Block::Block(float x, float y)
{
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float height = static_cast<float>(RenderingSystem::GetResolution().y);

	position = vec2(x, y);
	orig_position = vec2(x, y);
	// Half the pad's width, 1.5x the pad's height.
	size = vec2(width * 0.06f, height * 0.0520845);
	c = color(random(80, 255), random(80, 255), random(80, 255));
	c_mediumtones = color(red(c) / 2, green(c) / 2, blue(c) / 2);
	c_darktones = color(red(c) / 4, green(c) / 4, blue(c) / 4);
	displace_ratio = height / 45.0f;
}

void Block::update()
{
	if (RUMBLE) {
		position.x = orig_position.x + random(displace_ratio) - (displace_ratio / 2.0f);
		position.y = orig_position.y + random(displace_ratio) - (displace_ratio / 2.0f);

		c = color(random(80, 256), random(80, 256), random(80, 256));
		c_mediumtones = color(red(c) / 2, green(c) / 2, blue(c) / 2);
		c_darktones = color(red(c) / 4, green(c) / 4, blue(c) / 4);
	}

	topedge = position.y - (size.y / 2.0f);
	bottomedge = position.y + (size.y / 2.0f);
	leftedge = position.x - (size.x / 2.0f);
	rightedge = position.x + (size.x / 2.0f);
}

void Block::draw()
{
	noStroke();
	// A block is actually composed of four triangles with different colors.
	pushMatrix();
		translate(position.x, position.y);
		pushMatrix();
			fill(c);
			scale(1, -1);
			triangle(0.0f, 0.0f, -size.x / 2.0f, size.y / 2.0f, size.x / 2.0f, size.y / 2.0f);
			scale(1, -1);
			fill(c_darktones);
			triangle(0.0f, 0.0f, -size.x / 2.0f, size.y / 2.0f, size.x / 2.0f, size.y / 2.0f);
			rotate(HALF_PI);
			fill(c_mediumtones);
			triangle(0.0f, 0.0f, -size.y / 2.0f, size.x / 2.0f, size.y / 2.0f, size.x / 2.0f);
			scale(1, -1);
			triangle(0.0f, 0.0f, -size.y / 2.0f, size.x / 2.0f, size.y / 2.0f, size.x / 2.0f);
		popMatrix();
	popMatrix();
}
