#include "AndroidRobot.h"
#include <ctime>
#include <cstdlib>
#include <sstream>

jointstate::jointstate(vec2 position, float angle) {
	this->position = position; this->angle = angle;
}

joint::joint(vec2 position, float minAngle, float maxAngle, float angle) {
	this->position = position;
	min_angle = minAngle;
	max_angle = maxAngle;
	this->angle = angle;
	clamp(this->angle, minAngle, maxAngle);
	defaults = jointstate(this->position, this->angle);
	parent = nullptr;
}

void joint::resetState() {
	position = defaults.position;
	angle    = defaults.angle;
}

void joint::transformToParents() {
	if (parent) {
		parent->transformToParents();
		translate(parent->getPosition());
		rotate(degtorad(parent->getAngle() - 90.0f));
	}
}

void joint::translateTo() {
	transformToParents();
	translate(position.x, position.y);
}

void joint::rotateTo() {
	rotate(degtorad(angle + 90));
}

void joint::draw()
{
	pushMatrix();

		if (parent) {
			// Desenhe uma linha até a junta-pai e rotacione-se de acordo.
			stroke(255, 255, 0);
			transformToParents();
			line(0.0f, 0.0f, position.x, position.y);
		}
		translate(position.x, position.y);

		pushMatrix();
			fill(0, 0, 255);
			stroke(0, 0, 255);
			color(0, 0, 255);
			// Ponto
			ellipse(0.0f, 0.0f, 5.0f, 5.0f);
			// Limitador de ângulos
			noFill();
			stroke(0, 255, 0);
			arc(0.0f, 0.0f, 15.0f, 15.0f,
				degtorad(min_angle), degtorad(max_angle));
			popMatrix();

		// Ângulo atual
		rotate(degtorad(angle) + degtorad(90));
		pushMatrix();
			stroke(255, 0, 0);
			strokeWeight(3);
			line(0.0f, 0.0f, 0.0f, -25.0f);
			strokeWeight(1);
		popMatrix();
		
	popMatrix();
}

AndroidRobot::AndroidRobot(vec2 position)
{
	this->position = position;
	c = color(163, 201, 56);
	
	// Defaults de cada joint
	body_j = new joint(position, 225.0f, 315.0f, 270.0f);
	head_j = new joint(0.0f, 120.0f, 45.0f, 135.0f, 90.0f);

	l_antenna_j = new joint(33.0f, 63.0f, 0.0f, 180.0f, 45.0f);
	r_antenna_j = new joint(-33.0f, 63.0f, 0.0f, 180.0f, 135.0f);

	l_arm_j = new joint(110.0f, 96.0f, -360.0f, 360.0f, -90.0f);
	r_arm_j = new joint(-110.0f, 96.0f, -360.0f, 360.0f, 270.0f);

	l_leg_j = new joint(35.0f, -20.0f, -135.0f, 90.0f, -90.0f);
	r_leg_j = new joint(-35.0f, -20.0f, 90.0f, 315.0f, 270.0f);

	l_foot_j = new joint(0.0f, 90.0f, 90.0f, 225.0f, 180.0f);
	r_foot_j = new joint(0.0f, 90.0f, -45.0f, 90.0f, 0.0f);

	// Parenting
	head_j->setParent(body_j);
	l_antenna_j->setParent(head_j);
	r_antenna_j->setParent(head_j);
	l_arm_j->setParent(body_j);
	r_arm_j->setParent(body_j);
	l_leg_j->setParent(body_j);
	r_leg_j->setParent(body_j);
	l_foot_j->setParent(l_leg_j);
	r_foot_j->setParent(r_leg_j);

	// Randomize
	srand(time(nullptr));
}

AndroidRobot::~AndroidRobot()
{
	// We're using C++, so deallocation is unavoidable
	delete head_j;
	delete l_antenna_j;
	delete r_antenna_j;
	delete l_arm_j;
	delete r_arm_j;
	delete l_leg_j;
	delete r_leg_j;
	delete l_foot_j;
	delete r_foot_j;
}

void AndroidRobot::setAction(int type)
{
	if (type != ActionType) {
		body_j->resetState();
		head_j->resetState();
		l_antenna_j->resetState();
		r_antenna_j->resetState();
		l_arm_j->resetState();
		r_arm_j->resetState();
		l_leg_j->resetState();
		r_leg_j->resetState();
		l_foot_j->resetState();
		r_foot_j->resetState();
		movementCounter = 0.0f;
	}
	if (type < 0 || type > 2)
		type = 0;
	ActionType = type;
}

void AndroidRobot::action_wave()
{
	if (r_arm_j->getAngle() < 135.0f)
		movementCounter += arm_accel;
	else if (r_arm_j->getAngle() > 135.0f)
		movementCounter -= arm_accel;
	if (body_j->getAngle() > 263.0f)
		body_j->setAngle(body_j->getAngle() - 1.0f);
	else body_j->setAngle(263.0f);
	if (l_leg_j->getAngle() < -83.0f)
		l_leg_j->setAngle(l_leg_j->getAngle() + 1.0f);
	else l_leg_j->setAngle(-83.0f);
	if (l_arm_j->getAngle() < -83.0f)
		l_arm_j->setAngle(l_arm_j->getAngle() + 1.0f);
	else l_arm_j->setAngle(-83.0f);
	clamp(movementCounter, -arm_max_speed, arm_max_speed);
	r_arm_j->setAngle(r_arm_j->getAngle() + movementCounter);
}

void AndroidRobot::action_dance()
{
	float danceAngle = 22.5f;
	movementCounter += 0.1f;
	if (movementCounter > TAU) movementCounter -= TAU;
	head_j->setAngle(90.0f - (sin(movementCounter) * danceAngle / 2.0f));
	body_j->setAngle(270.0f + (sin(movementCounter) * danceAngle));
	l_antenna_j->setAngle(45.0f - (sin(movementCounter) * danceAngle));
	r_antenna_j->setAngle(135.0f - (sin(movementCounter) * danceAngle));
	l_arm_j->setAngle(-90.0f - (sin(movementCounter) * danceAngle));
	r_arm_j->setAngle(-90.0f - (sin(movementCounter) * danceAngle));
	l_leg_j->setAngle(-90.0f - (sin(movementCounter) * danceAngle));
	r_leg_j->setAngle(270.0f - (sin(movementCounter) * danceAngle));
}

void AndroidRobot::action_stand()
{
	// When standing, antennas should move slightly
	if (r_antenna_j->getAngle() < 112.5f)
		movementCounter += arm_accel / 100.0f;
	else if (r_antenna_j->getAngle() > 112.5f)
		movementCounter -= arm_accel / 100.0f;
	clamp(movementCounter, -arm_max_speed / 10.0f, arm_max_speed / 10.0f);
	r_antenna_j->setAngle(r_antenna_j->getAngle() + movementCounter);
	l_antenna_j->setAngle(180 - (r_antenna_j->getAngle() + movementCounter));
}

void AndroidRobot::update()
{
	body_j->setPosition(position);

	// Define widths and heights with factors.
	head_w = head_w_o + (head_w_o * head_f);
	head_h = head_h_o + (head_h_o * head_f);
	arm_w = arm_w_o;
	arm_h = arm_h_o + (arm_h_o * arm_f);
	leg_w = leg_w_o;
	leg_h = leg_h_o + (leg_h_o * leg_f);
	torso_w = torso_w_o + (torso_w_o * torso_f);
	torso_h = torso_h_o + (torso_h_o * torso_f);

	// Repositioning by scale
	// vs. Torso
	// Head
	head_j->setPosition(vec2(0.0f, 120.0f + (torso_h_o / 2.0f * torso_f)));

	// Arms
	l_arm_j->setPosition(vec2(110.0f + (torso_w_o / 2.0f * torso_f),
		96.0f + (torso_h_o / 2.0f * torso_f)));
	r_arm_j->setPosition(vec2(-110.0f - (torso_w_o / 2.0f * torso_f),
		96.0f + (torso_h_o / 2.0f * torso_f)));
	// Legs
	l_leg_j->setPosition(vec2(35.0f + (torso_w_o / 2.0f * torso_f),
		-20.0f - (torso_h_o / 2.0f * torso_f)));
	r_leg_j->setPosition(vec2(-35.0f - (torso_w_o / 2.0f * torso_f),
		-20.0f - (torso_h_o / 2.0f * torso_f)));
	// Feet vs. legs
	l_foot_j->setPosition(vec2(0.0f, leg_h));
	r_foot_j->setPosition(vec2(0.0f, leg_h));

	// Antennas vs. Head
	l_antenna_j->setPosition(vec2(33.0f + (head_w_o / 3.0f * head_f),
		63.0f + (head_h_o / 3.0f * head_f)));
	r_antenna_j->setPosition(vec2(-33.0f - (head_w_o / 3.0f * head_f),
		63.0f + (head_h_o / 3.0f * head_f)));

	switch (ActionType) {
	default:
		action_stand();
		break;
	case 1: // Wave arm
		action_wave();
		break;
	case 2: // Dance
		action_dance();
		break;
	}

	// Routine for eyes blinking.
	// See more at drawEyes.
	if (blinkCounter < 120) blinkCounter++;
	else {
		blinkCounter2++;
		if (blinkCounter2 > 7) {
			blinkCounter = int(rand() % 121);
			blinkCounter2 = 0;
		}
	}

	if (OficinaFramework::ScreenSystem::IsDebugActive())
	{
		std::ostringstream oss;
		oss.clear();
		oss << "Action:           " << ActionType << std::endl
			<< "Position:         " << position << std::endl
			<< "Movement Counter: " << movementCounter << std::endl
			<< "Blink:            " << blinkCounter << std::endl
			<< "Left leg:         " << l_leg_j->getPosition() << " @ " << l_leg_j->getAngle() << std::endl;
		OficinaFramework::ScreenSystem::Debug_AddLine(oss.str());
	}
}

void AndroidRobot::drawAntennas()
{
	// Left antenna
	pushMatrix();
		fill(c);
		stroke(c);
		l_antenna_j->translateTo();
		scale(-1, -1);
		l_antenna_j->rotateTo();
		rect(0, 0, 5.0f, antenna_h, 5.0f);
	popMatrix();

	// Right antenna
	pushMatrix();
		fill(c);
		stroke(c);
		r_antenna_j->translateTo();
		scale(-1, -1);
		r_antenna_j->rotateTo();
		rect(0, 0, 5.0f, antenna_h, 5.0f);
	popMatrix();
}

void AndroidRobot::drawHead()
{
	pushMatrix();
		head_j->translateTo();
		head_j->rotateTo();
		scale(1, -1);
		fill(c);
		stroke(c);
		arc(0, 0, head_w, head_h, 0, PI, PIE);
	popMatrix();
}

void AndroidRobot::drawArms()
{
	// Left arm
	pushMatrix();
		l_arm_j->translateTo();
		l_arm_j->rotateTo();
		fill(c);
		stroke(c);
		// adjust
		translate(0.0f, -5.0f);
		arc(0, 0, arm_w, 40.0f, 0, PI, PIE);
		scale(1, -1);
		translate(-arm_w / 2.0f, 0.0f);
		rect(0, 0, arm_w, arm_h);
		translate(arm_w / 2.0f, arm_h);
		arc(0, 0, arm_w, 40.0f, 0, PI, PIE);
	popMatrix();

	// Right arm
	pushMatrix();
		r_arm_j->translateTo();
		r_arm_j->rotateTo();
		fill(c);
		stroke(c);
		// adjust
		translate(0.0f, -5.0f);
		arc(0, 0, arm_w, 40.0f, 0, PI, PIE);
		scale(1, -1);
		translate(-arm_w / 2.0f, 0.0f);
		rect(0, 0, arm_w, arm_h);
		translate(arm_w / 2.0f, arm_h);
		arc(0, 0, arm_w, 40.0f, 0, PI, PIE);
	popMatrix();
}

void AndroidRobot::drawLegs()
{
	// Left leg
	pushMatrix();
		l_leg_j->translateTo();
		l_leg_j->rotateTo();
		fill(c);
		stroke(c);
		// adjust
		scale(1, -1);
		translate(-leg_w / 2.0f, -5.0f);
		rect(0, 0, leg_w, leg_h);
		translate(leg_w / 2.0f, leg_h);
		arc(0, 0, leg_w, 40.0f, 0, PI, PIE);
	popMatrix();

	// Right leg
	pushMatrix();
		r_leg_j->translateTo();
		r_leg_j->rotateTo();
		fill(c);
		stroke(c);
		// adjust
		scale(1, -1);
		translate(-leg_w / 2.0f, -5.0f);
		rect(0, 0, leg_w, leg_h);
		translate(leg_w / 2.0f, leg_h);
		arc(0, 0, leg_w, 40.0f, 0, PI, PIE);
	popMatrix();
}

void AndroidRobot::drawTorso()
{
	pushMatrix();
		body_j->translateTo();
		body_j->rotateTo();
		fill(c);
		stroke(c);
		translate(-80.0f, -110.0f);
		rect((-torso_w_o / 2.0f * torso_f), (-torso_h_o / 2.0f * torso_f),
			torso_w, torso_h, 0.0f, 0.0f, 50.0f, 50.0f);
	popMatrix();
}

void AndroidRobot::drawEyes()
{
	// blinkCounter < 120: Eyes open
	// blinkCounter >= 120: Eyes blinking
	float eyeHeight;
	if (blinkCounter < 120) eyeHeight = 10.0f;
	else eyeHeight = 1.0f;

	fill(255);
	stroke(255);
	pushMatrix();
		head_j->translateTo();
		head_j->rotateTo();
		pushMatrix();
			translate(-40 - (head_w_o / 3.0f * head_f),
				-40 - (head_h_o / 3.0f * head_f));
			ellipse(0.0f, 0.0f, 10.0f, eyeHeight);
		popMatrix();
		pushMatrix();
			translate(40 + (head_w_o / 3.0f * head_f),
				-40 - (head_h_o / 3.0f * head_f));
			ellipse(0.0f, 0.0f, 10.0f, eyeHeight);
		popMatrix();
	popMatrix();
}

void AndroidRobot::drawShoes()
{
	if (!visible_shoes) return;
	pushMatrix();
		l_foot_j->translateTo();
		l_foot_j->rotateTo();
		translate(-leg_w, -leg_w / 2.0f);
		noStroke();
		fill(80, 0, 0);
		rect(0.0f, -1.0f, (leg_w*1.5f), leg_w + 2);
		arc(0.0f, 0.0f, leg_w * 2.0f, leg_w * 2.0f, PI + HALF_PI, TAU);
		translate(-10.0f, -leg_w);
		fill(255);
		rect(0.0f, 0.0f, 10.0f, leg_w*2.0f);
	popMatrix();

	pushMatrix();
		r_foot_j->translateTo();
		r_foot_j->rotateTo();
		translate(leg_w, -leg_w / 2.0f);
		scale(-1, 1);
		noStroke();
		fill(80, 0, 0);
		rect(0.0f, -1.0f, (leg_w*1.5f), leg_w + 2);
		arc(0.0f, 0.0f, leg_w * 2.0f, leg_w * 2.0f, PI + HALF_PI, TAU);
		translate(-10.0f, -leg_w);
		fill(255);
		rect(0.0f, 0.0f, 10.0f, leg_w*2.0f);
	popMatrix();
}

void AndroidRobot::drawGlasses()
{
	if (!visible_glasses) return;
	pushMatrix();
		head_j->translateTo();
		head_j->rotateTo();
		noFill();
		strokeWeight(5);
		stroke(90);
		// Middle handle
		pushMatrix();
			translate(0.0f, -40 - (head_h_o / 3.0f * head_f));
			arc(0.0f, 0.0f, 30.0f + (head_h_o / 1.5f * head_f), 30.0f, PI, TAU);
		popMatrix();

		// Left
		pushMatrix();
			translate(-40 - (head_w_o / 3.0f * head_f),
				-40 - (head_h_o / 3.0f * head_f));
			ellipse(0.0f, 0.0f, 50.0f, 50.0f);
		popMatrix();
		// Right
		pushMatrix();
			translate(40 + (head_w_o / 3.0f * head_f),
				-40 - (head_h_o / 3.0f * head_f));
			ellipse(0.0f, 0.0f, 50.0f, 50.0f);
		popMatrix();
	popMatrix();
	strokeWeight(1);
}

void AndroidRobot::drawHat()
{
	if (!visible_hat) return;
	pushMatrix();
		head_j->translateTo();
		head_j->rotateTo();
		rotate(degtorad(-22.5));
		noStroke();
		fill(20);
		translate(0.0f, -80 - (head_h_o / 3.0f * head_f));
		ellipse(0.0f, 0.0f, 80.0f, 40.0f);
		stroke(255);
		noFill();
		ellipse(0.0f, 0.0f, 65.0f, 15.0f);
		noStroke();
		fill(20);
		ellipse(0.0f, 0.0f, 50.0f, 15.0f);
		fill(20);
		rect(-25.0f, -50.0f, 50.0f, 50.0f);
		stroke(90);
		ellipse(0.0f, -50.0f, 50.0f, 15.0f);
	popMatrix();
}

void AndroidRobot::drawClockwatch()
{
	if (!visible_clockwatch) return;
	pushMatrix();
		l_arm_j->translateTo();
		l_arm_j->rotateTo();
		scale(1, -1);
		noStroke();
		translate(0.0f, arm_h - 5.0f);
		fill(20);
		rect(-arm_w / 2.0f - 1.0f, 0.0f, arm_w + 2.0f, 10.0f);
		fill(135, 206, 235);
		translate(0.0f, 5.0f);
		ellipse(0.0f, 0.0f, 20.0f, 20.0f);
		stroke(90);
		strokeWeight(3);
		line(0.0f, 0.0f, -8.0f, 0.0f);
		strokeWeight(1);
		line(0.0f, 0.0f, 0.0f, 8.0f);
		fill(240, 255, 255);
		noStroke();
		ellipse(-1.0f, 1.0f, 5.0f, 5.0f);
	popMatrix();
}

void AndroidRobot::draw_joints()
{
	l_antenna_j->draw();
	r_antenna_j->draw();
	head_j->draw();
	l_arm_j->draw();
	r_arm_j->draw();
	l_foot_j->draw();
	r_foot_j->draw();
	l_leg_j->draw();
	r_leg_j->draw();
	body_j->draw();
}

void AndroidRobot::draw_robot()
{
	drawAntennas();
	drawHead();
	drawHat();
	drawEyes();
	drawGlasses();
	drawLegs();
	drawShoes();
	drawTorso();
	drawArms();
	drawClockwatch();
}

void AndroidRobot::draw()
{
	if (visible)
		draw_robot();
	if (visible_joints)
		draw_joints();
}
