#pragma once
#include "util.h"
class BreakoutApp :
	public Processing2DWindow
{
public:
	virtual void setup() override;
	virtual void unload() override;
	void updateInput();
	virtual void draw() override;
};

