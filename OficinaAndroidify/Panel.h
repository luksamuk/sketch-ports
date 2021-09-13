#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include "util.h"
#include "AndroidRobot.h"

class ScrollBar
{
private:
	vec2 position;
	float sliderPosition = 0.0f;
	float sliderSize = 250.0f;
	float factor = 1.0f;
	bool dragged = false;
public:
	ScrollBar(float xpos, float ypos);

	bool  isMouseOver(vec2);
	float getFactor() { return factor; }
	bool  getDragged() { return dragged; }

	void setFactor(float val);
	void setDragged(bool state) { dragged = state; }

	void recalcFactor();
	void update();
	void draw();
};

class Panel
{
private:
	const float panel_accel = 2.0f;

	vec2 position;
	bool showing = false;
	float panel_xspeed = 0.0f;

	ScrollBar* headSlider,
			 * torsoSlider,
			 * armsSlider,
			 * legsSlider;
	float menu_button_angle;

	AndroidRobot* r;

public:
	Panel(AndroidRobot* r, int width, int height);
	~Panel();

	vec2 getPosition() { return position; }
	
	void togglePanel();
	void evalClick(vec2 clickpos);
	void evalUnclick(vec2 clickpos);
	void update(int, int);
	void draw();
};

const color dark_red = color(211, 47, 47),
			redcolor = color(244, 67, 54),
			deep_orange = color(255, 87, 34),
			text_icons = color(255, 255, 255),
			primary_text = color(33, 33, 33),
			secondary_text = color(114, 114, 114),
			divider = color(182, 182, 182),
			light_primary = color(255, 205, 210),

			android_green = color(163, 201, 56),
			marine_blue = color(0, 51, 102),
			water_green = color(26, 227, 171),
			purple = color(116, 12, 110),
			royal_red = color(116, 12, 12),
			yellow = color(237, 206, 19),
			pink = color(255, 125, 236),
			baby_blue = color(137, 207, 240),
			orange = color(255, 153, 0);
