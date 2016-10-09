#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include "util.h"

struct jointstate
{
	vec2 position;
	float angle;
	jointstate(vec2 position, float angle);
	jointstate() : position(vec2::Zero()), angle(0.0f) {}
};

class joint
{
private:
	float max_angle = 360.0f;
	float min_angle = 180.0f;

	vec2       position;
	float      angle;
	joint*     parent;
	jointstate defaults;
public:
	joint(vec2 position, float minAngle, float maxAngle, float angle);
	joint(float xpos, float ypos, float minAngle, float maxAngle, float angle)
		: joint(vec2(xpos, ypos), minAngle, maxAngle, angle) {}

	void setParent(joint* j) { parent = j;  }
	void setPosition(vec2 position) { this->position = position;  }
	void setAngle(float angle) { clamp(angle, min_angle, max_angle);  this->angle = angle; }

	vec2   getPosition() { return position; }
	float  getAngle() { return angle;  }
	joint* getParent() { return parent; }

	void resetState();
	void transformToParents();
	void translateTo();
	void rotateTo();

	void draw();
};


class AndroidRobot
{
private:
	vec2 position;

	// Joints
	joint* body_j,
		*head_j,
		*l_antenna_j,
		*r_antenna_j,
		*l_arm_j,
		*r_arm_j,
		*l_leg_j,
		*r_leg_j,
		*l_foot_j,
		*r_foot_j;

	// Original sizes
	const float head_w_o = 160.0f,
		head_h_o         = 160.0f,
		antenna_h        = 30.0f,
		arm_w_o          = 40.0f,
		arm_h_o          = 80.0f,
		leg_w_o          = 40.0f,
		leg_h_o          = 80.0f,
		torso_w_o        = 160.0f,
		torso_h_o        = 160.0f;

	// Current sizes
	float head_w,
		head_h,
		arm_w,
		arm_h,
		leg_w,
		leg_h,
		torso_w,
		torso_h;

	//  Increase-size factors
	float head_f,
		arm_f,
		leg_f,
		torso_f;

	// States
	bool visible = true,
		visible_joints = false,
		visible_shoes = false,
		visible_glasses = false,
		visible_hat = false,
		visible_clockwatch = false;

	// Color
	color c;

	// Action stuff
	int ActionType;
	float movementCounter;
	const float arm_max_speed = 10.0f;
	const float arm_accel = 2.5f;
	int blinkCounter,
		blinkCounter2;

public:
	// ctor + dtor
	AndroidRobot(vec2 position);
	~AndroidRobot();

	// Setters
	void setPosition(vec2 position) { this->position = position; }
	void setColor(color c) { this->c = c; }
	void setHeadFactor(float val) { clamp(val, -1.0f, 1.0f); head_f = val; }
	void setTorsoFactor(float val) { clamp(val, -1.0f, 1.0f); torso_f = val; }
	void setArmsFactor(float val) { clamp(val, -1.0f, 1.0f);  arm_f = val; }
	void setLegsFactor(float val) { clamp(val, -1.0f, 1.0f);  leg_f = val; }
	void setAction(int type);
	void setShoesVisible(bool val) { visible_shoes = val; }
	void setGlassesVisible(bool val) { visible_glasses = val; }
	void setHatVisible(bool val) { visible_hat = val; }
	void setClockwatchVisible(bool val) { visible_clockwatch = val; }
	void setVisible(bool val) { visible = val; }
	void setVisibleJoints(bool val) { visible_joints = val; }


	// Getters
	int getAction() { return ActionType; }
	bool getShoesVisible() { return visible_shoes; }
	bool getGlassesVisible() { return visible_glasses; }
	bool getHatVisible() { return visible_hat; }
	bool getClockwatchVisible() { return visible_clockwatch; }
	bool getVisible() { return visible; }
	bool getVisibleJoints() { return visible_joints; }

	// Behavior
	void action_wave();
	void action_dance();
	void action_stand();

	void update();

	// Drawing
	void drawAntennas();
	void drawHead();
	void drawArms();
	void drawLegs();
	void drawTorso();
	void drawEyes();
	void drawShoes();
	void drawGlasses();
	void drawHat();
	void drawClockwatch();

	void draw_joints();
	void draw_robot();

	void draw();

};