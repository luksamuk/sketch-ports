#include <OficinaFramework\TimingSystem.hpp>
#include "Ball.h"

extern int  LIVES;
extern bool RESET_GAME;
extern int  SCORE;
extern bool RUMBLE;
extern int  MULTIPLIER;

Ball::Ball()
{
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float height = static_cast<float>(RenderingSystem::GetResolution().y);

	position = vec2(0.0f, 21.0f * height / 24.0f);
	maxspeed = height / 72.0f; // Which gives us 10.0f under 720p
	speed = vec2::Zero();
	stopped = true;
	diameter = height * 0.034723f;
}

void Ball::TrackPad(Pad* p) {
	trackedPad = p;
}

void Ball::TrackBlocks(std::list<Block*>* b) {
	trackedBlocks = b;
}

void Ball::wakeUp() {
	if (stopped) {
		stopped = false;
		float initial_angle = 67.5f + random(46);
		speed.x = maxspeed * cosf(degtorad(initial_angle));
		speed.y = -maxspeed * sinf(degtorad(initial_angle));
	}
}

void Ball::update()
{
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float height = static_cast<float>(RenderingSystem::GetResolution().y);
	float frameRate = static_cast<float>(TimingSystem::GetFPS());

	// If currently stopped, then there is no point in verifying
	// all of this.
	// Just follow the pad.
	if (stopped) {
		if (trackedPad != nullptr)
			position.x = trackedPad->position.x;
		return;
	}

	// Movement
	// Store old position for afterimage
	afterImages.push_back(vec2(position.x, position.y));

	// Also, remove afterimages accordingly.
	if (afterImages.size() >= 7) {
		afterImages.pop_front();
	}

	// Limits for speed
	speed.clamp(vec2(-maxspeed), vec2(maxspeed));

	// Adjust movement to framerate-independent.
	position.x += speed.x * 60.0f / frameRate;
	position.y += speed.y * 60.0f / frameRate;


	// Aliases for boundaries
	topedge = position.y - (diameter / 2.0f);
	bottomedge = position.y + (diameter / 2.0f);
	leftedge = position.x - (diameter / 2.0f);
	rightedge = position.x + (diameter / 2.0f);

	// Boundary collisions
	// Top
	if (topedge <= 0.0f && speed.y < 0.0f)
		speed.y *= -1.0f;
	// Left and right
	else if ((leftedge <= 0.0f && speed.x < 0.0f) || (rightedge >= width && speed.x > 0.0f))
		speed.x *= -1.0f;

	// Pad x Ball collision
	if (trackedPad)
	{
		// Common initial collision; debug only
		// Am I on the required height?
		if (bottomedge >= trackedPad->topedge && bottomedge <= trackedPad->bottomedge && speed.y > 0.0f)
		{
			if (rightedge >= trackedPad->leftedge && leftedge <= trackedPad->rightedge)
			{
				// Now, pad physics.
				// First, let's calculate a ratio that will give us
				// the actual desired angle of the ball, depending on where the ball hits the pad.
				// The final angle is supposed to be betweeen 45 and 135 degrees, right and left
				// borders respectively.
				float ratio = (-2.0f*(position.x - trackedPad->leftedge) / (trackedPad->rightedge - trackedPad->leftedge)) + 1.0f;

				// Calculate theta
				theta = 90.0f + (ratio * 45.0f);

				// Set the speed to respective values
				speed.x = maxspeed * cosf(degtorad(theta));
				speed.y = -maxspeed * sinf(degtorad(theta));

				// Reset multiplier
				if(MULTIPLIER >= 2)
					SCORE += static_cast<int>(powf(2, MULTIPLIER));
				MULTIPLIER = 0;
			}
		}
	}
	// if we've gone beyond the bottom, then respawn.
	if (topedge > height) {
		LIVES--;
		MULTIPLIER = 0;
		// Game Over and such.
		if (LIVES <= 0) {
			RESET_GAME = true;
		}

		respawn();
	}

	// Blocks: Verify for collision on list.
	if (trackedBlocks)
	{
		for (Block* b : *trackedBlocks)
		{
			// Block physics are actually simpler.
			// All you need to do is invert X and Y accordingly, plus remove if necessary.
			if (intersectsBlock(b))
			{
				boolean touched = false;
				// Left collision
				if ((rightedge >= b->leftedge && rightedge < b->position.x && speed.x > 0.0f)
					// Right collision
					|| (leftedge <= b->rightedge && leftedge > b->position.x && speed.x < 0.0f)) {
					speed.x *= -1.0f;
					touched = true;
				}

				// Top collision
				if ((bottomedge >= b->topedge && bottomedge < b->position.y && speed.y > 0.0f)
					// Bottom collision
					|| (topedge <= b->bottomedge && topedge > b->position.y && speed.y < 0.0f)) {
					speed.y *= -1.0f;
					touched = true;
				}

				if (touched)
				{
					// The credit to who's due.
					int gain = RUMBLE ? 20 : 10;
					SCORE += gain + (MULTIPLIER >= 2 ? gain * MULTIPLIER : 0);
					if(MULTIPLIER < 4) MULTIPLIER++;

					// It's a possibility, of course, that, on smaller framerates,
					// the block and the ball could be at (0, 0), but in that case,
					// your PC is probably a toaster for running like, under 15FPS.

					// Also remove block from collection (sorta)
					removalBlocks.push(b);
				}
			}
		}

		// Remove block from collection (truly)
		while (!removalBlocks.empty())
		{
			delete removalBlocks.top();
			trackedBlocks->remove(removalBlocks.top());
			removalBlocks.pop();
		}
	}
}

void Ball::respawn() {
	float height = static_cast<float>(RenderingSystem::GetResolution().y);

	stopped = true;
	speed.x = speed.y = 0.0f;
	position.y = 21.0f * height / 24.0f;
	if (trackedPad != nullptr)
		position.x = trackedPad->position.x;
	afterImages.clear();
}

void Ball::draw()
{
	// After-image
	int opacity = 2;
	for (vec2 v : afterImages) {
		pushMatrix();
		opacity *= 2;
		fill(80, 80, 80, opacity);
		translate(v.x, v.y);
		ellipse(0.0f, 0.0f, diameter, diameter);
		popMatrix();
	}

	// Actual ball
	fill(255);
	pushMatrix();
	translate(position.x, position.y);
	ellipse(0.0f, 0.0f, diameter, diameter);
	popMatrix();
}

bool Ball::intersectsBlock(Block* b)
{
	vec2 compare_point = position;
	compare_point.clamp(vec2(b->leftedge, b->topedge),
		vec2(b->rightedge, b->bottomedge));

	if (vec2::distance(position, compare_point) <= diameter / 2.0f)
		return true;
	return false;
}