#pragma once
#include "util.h"
class MainMenuApp :
	public Processing2DWindow
{
public:
	virtual void setup() override;
	virtual void unload() override;
	virtual void draw() override;
};