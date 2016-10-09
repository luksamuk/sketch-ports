#pragma once
#include <OficinaFramework\OficinaTypes.hpp>
#include "util.h"
#include <list>
#include <deque>
#include <stack>
using namespace OficinaFramework;

#include "Block.h"
#include "Pad.h"

class Ball
{
public:
	bool stopped;
	vec2 position;
	vec2 speed;
	float diameter;
	Pad* trackedPad = nullptr;
	std::list<Block*>* trackedBlocks = nullptr;
	std::stack<Block*> removalBlocks;
	std::deque<vec2> afterImages;
	float maxspeed;

	float topedge,
		bottomedge,
		leftedge,
		rightedge;

	float theta;

	Ball();
	void TrackPad(Pad* p);
	void TrackBlocks(std::list<Block*>* b);
	void wakeUp();
	void update();
	void respawn();
	void draw();
	bool intersectsBlock(Block* b);
};