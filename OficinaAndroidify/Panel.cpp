#include "Panel.h"
#include <OficinaFramework\InputSystem.hpp>
#include <OficinaFramework\RenderingSystem.hpp>
using namespace OficinaFramework;

extern bool BACKTOMENU;

ScrollBar::ScrollBar(float xpos, float ypos)
{
	position = vec2(xpos, ypos);
	sliderPosition = sliderSize / 2.0f;
	recalcFactor();
}

bool ScrollBar::isMouseOver(vec2 oldmousepos)
{
	vec2 mousepos = oldmousepos;
	mousepos -= position;
	return(mousepos.x >= sliderPosition - 2.5f &&
		   mousepos.x <= sliderPosition + 2.5f &&
		   mousepos.y >= -5.0f &&
		   mousepos.y <= 5.0f);
}

void ScrollBar::setFactor(float val)
{
	factor = val;
	clamp(factor, -1.0f, 1.0f);
	sliderPosition = sliderSize * ((factor + 1.0f) / 2.0f);
}

void ScrollBar::recalcFactor() {
	factor = ((sliderPosition / sliderSize) * 2.0f) - 1.0f;
}

void ScrollBar::update()
{
	vec2 mouse = OficinaFramework::InputSystem::GetMousePos().toVec2();
	if (dragged) {
		sliderPosition = mouse.x - position.x;
		clamp(sliderPosition, 0.0f, sliderSize);
		recalcFactor();
	}
}

void ScrollBar::draw()
{
	pushMatrix();
		translate(position.x, position.y);
		stroke(secondary_text);
		fill(secondary_text);
		// Beginning
		line(0.0f, -5.0f, 0.0f, 5.0f);
		// Scroll line
		line(0.0f, 0.0f, sliderSize, 0.0f);
		// Middle
		line(sliderSize / 2.0f, -5.0f, sliderSize / 2.0f, 5.0f);
		// End
		line(sliderSize, -5.0f, sliderSize, 5.0f);
		// Slider
		pushMatrix();
			translate(sliderPosition, 0.0f);
			rect(-2.5f, -5.0f, 5.0f, 10.0f);
		popMatrix();
		noStroke();
	popMatrix();
}

Panel::Panel(AndroidRobot * r, int width, int height)
{
	position = vec2(-width/2, 0.0f);
	this->r = r;
	headSlider = new ScrollBar(70 + (width / 32.0f),  25 * (height / 32.0f) + 5.0f);
	torsoSlider = new ScrollBar(70 + (width / 32.0f), 26 * (height / 32.0f) + 5.0f);
	armsSlider = new ScrollBar(70 + (width / 32.0f),  27 * (height / 32.0f) + 5.0f);
	legsSlider = new ScrollBar(70 + (width / 32.0f),  28 * (height / 32.0f) + 5.0f);
}

Panel::~Panel()
{
	delete headSlider;
	delete torsoSlider;
	delete armsSlider;
	delete legsSlider;
}

void Panel::togglePanel() {
	showing = !showing;
	panel_xspeed = 0.0f;
}

void Panel::evalClick(vec2 clickpos)
{
	float width = RenderingSystem::GetResolution().x;
	float height = RenderingSystem::GetResolution().y;

	// If you're hidden, then show.
	if (!showing) {
		if (getDistance(clickpos.x, clickpos.y, 30.0f, 30.0f) <= 25.0f)
			showing = true;
		panel_xspeed = 0.0f;
	}

	// If you wanna go back, then... bye.
	if (getDistance(clickpos.x, clickpos.y, width - 30.0f, height - 30.0f) <= 25.0f)
	{
		BACKTOMENU = true;
	}

	// Do not change anything unless you're
	// stopped and shown.
	if (position.x != 0.0f) return;

	// Hide if that's the case
	if (getDistance(clickpos.x, clickpos.y, (width / 2.0f) + 30.0f, 30.0f) <= 25.0f) {
		showing = false;
		panel_xspeed = 0.0f;
	}

	// Fields
	// Toggle robot view
	if (getDistance(clickpos.x, clickpos.y, (width / 64.0f), 10 * (height / 32.0f) + 2)
		<= 7.5f)
		r->setVisible(!r->getVisible());
	// Toggle joint view
	if (getDistance(clickpos.x, clickpos.y, (width / 64.0f), 11 * (height / 32.0f) + 2)
		<= 7.5f)
		r->setVisibleJoints(!r->getVisibleJoints());


	// Change color
	// Android Green
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 10.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(android_green);
	// Water Green
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 44.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(water_green);
	// Purple
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 80.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(purple);
	// Royal Red
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 115.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(royal_red);
	// Yellow
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 150.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(yellow);
	// Pink
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 185.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(pink);
	// Marine Blue
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 220.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(marine_blue);

	// Baby Blue
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 255.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(baby_blue);

	// Orange
	if (getDistance(clickpos.x, clickpos.y, (width / 32.0f) + 290.0f,
		14 * (height / 32.0f) + 8.0f) <= 15.0f)
		r->setColor(orange);

	// Toggle acessories
	// Hat
	if (clickpos.x >= (width / 32.0f) - 5.0f
		&& clickpos.x <= (width / 32.0f) - 5.0f + 70.0f
		&& clickpos.y >= 18 * (height / 32.0f) - 9.0f
		&& clickpos.y <= (18 * (height / 32.0f) - 9.0f) + 24.0f)
		r->setHatVisible(!r->getHatVisible());
	// Clockwatch
	if (clickpos.x >= 90 + (width / 32.0f) - 5.0f
		&& clickpos.x <= (90 + (width / 32.0f) - 5.0f) + 70.0f
		&& clickpos.y >= 18 * (height / 32.0f) - 9.0f
		&& clickpos.y <= (18 * (height / 32.0f) - 9.0f) + 24.0f)
		r->setClockwatchVisible(!r->getClockwatchVisible());
	// Shoes
	if (clickpos.x >= 180 + (width / 32.0f) - 5.0f
		&& clickpos.x <= (180 + (width / 32.0f) - 5.0f) + 70.0f
		&& clickpos.y >= 18 * (height / 32.0f) - 9.0f
		&& clickpos.y <= (18 * (height / 32.0f) - 9.0f) + 24.0f)
		r->setShoesVisible(!r->getShoesVisible());
	// Glasses
	if (clickpos.x >= 270 + (width / 32.0f) - 5.0f
		&& clickpos.x <= (270 + (width / 32.0f) - 5.0f) + 70.0f
		&& clickpos.y >= 18 * (height / 32.0f) - 9.0f
		&& clickpos.y <= (18 * (height / 32.0f) - 9.0f) + 24.0f)
		r->setGlassesVisible(!r->getGlassesVisible());

	// Change animation
	// None
	if (clickpos.x >= (width / 32.0f)
		&& clickpos.y >= 22 * (height / 32.0f) - 10
		&& clickpos.x <= ((width / 32.0f) + 65)
		&& clickpos.y <= 22 * (height / 32.0f) + 12) {
		r->setAction(0);
	}
	// Wave
	if (clickpos.x >= 6 * (width / 32.0f)
		&& clickpos.y >= 22 * (height / 32.0f) - 10
		&& clickpos.x <= (6 * (width / 32.0f) + 55)
		&& clickpos.y <= 22 * (height / 32.0f) + 12) {
		r->setAction(1);
	}
	// Dance
	if (clickpos.x >= 11 * (width / 32.0f)
		&& clickpos.y >= 22 * (height / 32.0f) - 10
		&& clickpos.x <= (11 * (width / 32.0f) + 55)
		&& clickpos.y <= 22 * (height / 32.0f) + 12) {
		r->setAction(2);
	}

	// Proportion sliders
	// Reset all
	// 30*-9, s24
	if (clickpos.x >= (width / 32.0f)
		&& clickpos.y >= 30 * (height / 32.0f) - 9.0f
		&& clickpos.x <= ((width / 32.0f) + 80)
		&& clickpos.y <= 30 * (height / 32.0f) + 24.0f) {
		headSlider->setFactor(0.0f);
		torsoSlider->setFactor(0.0f);
		armsSlider->setFactor(0.0f);
		legsSlider->setFactor(0.0f);
	}

	// Sliders
	if (headSlider->isMouseOver(clickpos))
		headSlider->setDragged(true);

	if (torsoSlider->isMouseOver(clickpos))
		torsoSlider->setDragged(true);

	if (armsSlider->isMouseOver(clickpos))
		armsSlider->setDragged(true);

	if (legsSlider->isMouseOver(clickpos))
		legsSlider->setDragged(true);
}

void Panel::evalUnclick(vec2 clickpos)
{
	headSlider->setDragged(false);
	torsoSlider->setDragged(false);
	armsSlider->setDragged(false);
	legsSlider->setDragged(false);
}

void Panel::update(int width, int height)
{
	clamp(panel_xspeed, -20.0f, 20.0f);
	if (showing) {
		if (position.x < 0.0f) {
			panel_xspeed += panel_accel;
			position.x += panel_xspeed;
		}
		else {
			position.x = 0.0f;
			panel_xspeed = 0.0f;
		}
	}
	else {
		if (position.x > -width / 2.0f) {
			panel_xspeed -= panel_accel;
			position.x += panel_xspeed;
		}
		else {
			position.x = -width / 2.0f;
			panel_xspeed = 0.0f;
		}
	}

	headSlider->update();
	torsoSlider->update();
	armsSlider->update();
	legsSlider->update();

	r->setHeadFactor(headSlider->getFactor());
	r->setTorsoFactor(torsoSlider->getFactor());
	r->setArmsFactor(armsSlider->getFactor());
	r->setLegsFactor(legsSlider->getFactor());

	// The three dots on the menu button should rotate when the panel
	// opens.
	menu_button_angle = ((position.x + (width / 2.0f)) / (width / 2.0f))
		* -HALF_PI;
}

void Panel::draw()
{
	float width = RenderingSystem::GetResolution().x;
	float height = RenderingSystem::GetResolution().y;

	textAlign(CENTER, CENTER);
	pushMatrix();
	translate(position.x, position.y);
	noStroke();
	// Panel button
	fill(deep_orange);
	ellipse((width / 2.0f) + 30.0f, 30.0f, 50.0f, 50.0f);

	fill(text_icons);
	pushMatrix();
	translate((width / 2.0f) + 30.0f, 30.0f);
	rotate(menu_button_angle);
	ellipse(0.0f, -10.0f, 5.0f, 5.0f);
	ellipse(0.0f, 0.0f, 5.0f, 5.0f);
	ellipse(0.0f, 10.0f, 5.0f, 5.0f);
	popMatrix();

	// Back button
	pushMatrix();
	translate(-position.x, -position.y);
	fill(royal_red);
	ellipse(width - 30.0f, height - 30.0f, 50.0f, 50.0f);
	stroke(text_icons);
		pushMatrix();
			translate(width - 30.0f, height - 30.0f);
			strokeWeight(3.0f);
			pushMatrix();
				rotate(45.0f);
				line(-15.0f, 0.0f, 15.0f, 0.0f);
			popMatrix();

			pushMatrix();
				rotate(-45.0f);
				line(-15.0f, 0.0f, 15.0f, 0.0f);
			popMatrix();
		popMatrix();
		strokeWeight(1.0f);
	popMatrix();
	stroke(0);
	fill(0);
	noStroke();

	// Panel background
	fill(text_icons);
	rect(-50, 0, (width / 2.0f) + 50.0f, height);

	// Panel headers
	fill(dark_red);
	rect(-50, 0, (width / 2.0f) + 50.0f, height / 20.0f);
	fill(redcolor);
	rect(-50, (height / 20.0f), (width / 2.0f) + 50.0f, height / 5.0f);


	// Text
	textAlign(CENTER, CENTER);
	textSize(32);
	fill(text_icons);
	text("Preferencias", (width / 4.0f), 3 * (height / 20.0f));


	// First section: Misc
	textAlign(LEFT, CENTER);
	textSize(16);
	fill(primary_text);
	text("Variados", (width / 32.0f), 9 * (height / 32.0f));
	fill(secondary_text);
	textSize(14);
	text("Mostrar robo", (width / 32.0f), 10 * (height / 32.0f));
	text("Mostrar esqueleto", (width / 32.0f), 11 * (height / 32.0f));

	// Checkboxes, first section
	ellipse((width / 64.0f), 10 * (height / 32.0f) + 2, 15.0f, 15.0f);
	ellipse((width / 64.0f), 11 * (height / 32.0f) + 2, 15.0f, 15.0f);
	fill(text_icons);
	if (r->getVisible())
		ellipse((width / 64.0f), 10 * (height / 32.0f) + 2, 10.0f, 10.0f);
	if (r->getVisibleJoints())
		ellipse((width / 64.0f), 11 * (height / 32.0f) + 2, 10.0f, 10.0f);

	// Divider
	stroke(divider);
	line(-10, 12 * (height / 32.0f), (width / 2.0f) - 1.0f,
		12 * (height / 32.0f));
	noStroke();

	// Second section: Colors
	textAlign(LEFT, CENTER);
	textSize(16);
	fill(primary_text);
	text("Cores", (width / 32.0f), 13 * (height / 32.0f));


	// Android Green
	fill(android_green);
	ellipse((width / 32.0f) + 10.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Water Green
	fill(water_green);
	ellipse((width / 32.0f) + 44.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Purple
	fill(purple);
	ellipse((width / 32.0f) + 80.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Royal Red
	fill(royal_red);
	ellipse((width / 32.0f) + 115.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Yellow
	fill(yellow);
	ellipse((width / 32.0f) + 150.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Pink
	fill(pink);
	ellipse((width / 32.0f) + 185.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Marine Blue
	fill(marine_blue);
	ellipse((width / 32.0f) + 220.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Baby Blue
	fill(baby_blue);
	ellipse((width / 32.0f) + 255.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Orange
	fill(orange);
	ellipse((width / 32.0f) + 290.0f, 14 * (height / 32.0f) + 8.0f, 30.0f,
		30.0f);

	// Divider
	stroke(divider);
	line(-10, 16 * (height / 32.0f), (width / 2.0f) - 1.0f,
		16 * (height / 32.0f));
	noStroke();

	// Third section: Outfits
	textAlign(LEFT, CENTER);
	textSize(16);
	fill(primary_text);
	text("Acessorios", (width / 32.0f), 17 * (height / 32.0f));

	fill(light_primary);
	rect((width / 32.0f) - 5.0f, 18 * (height / 32.0f) - 9.0f, 70.0f, 24.0f);
	fill(secondary_text);
	text("Chapeu", (width / 32.0f), 18 * (height / 32.0f));

	fill(light_primary);
	rect(90 + (width / 32.0f) - 5.0f, 18 * (height / 32.0f) - 9.0f, 70.0f,
		24.0f);
	fill(secondary_text);
	text("Relogio", 90 + (width / 32.0f), 18 * (height / 32.0f));

	fill(light_primary);
	rect(180 + (width / 32.0f) - 5.0f, 18 * (height / 32.0f) - 9.0f, 70.0f,
		24.0f);
	fill(secondary_text);
	text("Sapatos", 180 + (width / 32.0f), 18 * (height / 32.0f));

	fill(light_primary);
	rect(270 + (width / 32.0f) - 5.0f, 18 * (height / 32.0f) - 9.0f, 65.0f,
		24.0f);
	fill(secondary_text);
	text("Oculos", 270 + (width / 32.0f), 18 * (height / 32.0f));

	// Highlight acessories that are onscreen
	stroke(deep_orange);
	strokeWeight(2);
	// Hat
	if (r->getHatVisible())
		line((width / 32.0f) - 5.0f, 18 * (height / 32.0f) + 12,
			(width / 32.0f) + 62.0f, 18 * (height / 32.0f) + 12);
	// Clock
	if (r->getClockwatchVisible())
		line((width / 32.0f) + 90.0f - 5.0f, 18 * (height / 32.0f) + 12,
			(width / 32.0f) + 90.0f + 62.0f, 18 * (height / 32.0f) + 12);
	// Shoes
	if (r->getShoesVisible())
		line((width / 32.0f) + 180.0f - 5.0f, 18 * (height / 32.0f) + 12,
			(width / 32.0f) + 180.0f + 62.0f, 18 * (height / 32.0f) + 12);
	// Glasses
	if (r->getGlassesVisible())
		line((width / 32.0f) + 270.0f - 5.0f, 18 * (height / 32.0f) + 12,
			(width / 32.0f) + 270.0f + 62.0f, 18 * (height / 32.0f) + 12);
	strokeWeight(1);

	// Divider
	stroke(divider);
	line(-10, 20 * (height / 32.0f), (width / 2.0f) - 1.0f,
		20 * (height / 32.0f));
	noStroke();

	// Fourth section: Animations
	textAlign(LEFT, CENTER);
	textSize(16);
	fill(primary_text);
	text("Animacoes", (width / 32.0f), 21 * (height / 32.0f));

	fill(light_primary);
	rect((width / 32.0f) - 5.0f, 22 * (height / 32.0f) - 9.0f, 77.0f,
		24.0f);
	fill(secondary_text);
	text("Nenhum", (width / 32.0f), 22 * (height / 32.0f));
	fill(light_primary);
	rect(6 * (width / 32.0f) - 5.0f, 22 * (height / 32.0f) - 9.0f, 65.0f,
		24.0f);
	fill(secondary_text);
	text("Acenar", 6 * (width / 32.0f), 22 * (height / 32.0f));
	fill(light_primary);
	rect(11 * (width / 32.0f) - 5.0f, 22 * (height / 32.0f) - 9.0f, 65.0f,
		24.0f);
	fill(secondary_text);
	text("Dancar", 11 * (width / 32.0f), 22 * (height / 32.0f));

	// Select current animation.
	stroke(deep_orange);
	float anim_s_pos;
	float anim_s_size;
	switch (r->getAction()) {
	default:
		anim_s_pos = 1.0f;
		anim_s_size = 65.0f;
		break;
	case 1: // Wave
		anim_s_pos = 6.0f;
		anim_s_size = 55.0f;
		break;
	case 2: // Dance
		anim_s_pos = 11.0f;
		anim_s_size = 55.0f;
		break;
	}
	strokeWeight(2);
	line(anim_s_pos*(width / 32.0f), 22 * (height / 32.0f) + 12,
		anim_s_pos*(width / 32.0f) + anim_s_size,
		22 * (height / 32.0f) + 12);
	strokeWeight(1);

	// Divider
	stroke(divider);
	line(-10, 23 * (height / 32.0f), (width / 2.0f) - 1.0f,
		23 * (height / 32.0f));
	noStroke();

	// Fifth section: Proportions
	textAlign(LEFT, CENTER);
	textSize(16);
	fill(primary_text);
	text("Proporcoes", (width / 32.0f), 24 * (height / 32.0f));

	// Fields
	fill(secondary_text);
	text("Cabeca", (width / 32.0f), 25 * (height / 32.0f));
	text("Torso", (width / 32.0f), 26 * (height / 32.0f));
	text("Bracos", (width / 32.0f), 27 * (height / 32.0f));
	text("Pernas", (width / 32.0f), 28 * (height / 32.0f));

	// Scrolling bars
	headSlider->draw();
	torsoSlider->draw();
	armsSlider->draw();
	legsSlider->draw();

	// Reset button
	fill(light_primary);
	rect((width / 32.0f) - 5.0f, 30 * (height / 32.0f) - 9.0f, 80.0f, 24.0f);
	fill(secondary_text);
	text("Redefinir", (width / 32.0f), 30 * (height / 32.0f));

	// Divider
	stroke(divider);
	line(-10, 31 * (height / 32.0f), (width / 2.0f) - 1.0f,
		31 * (height / 32.0f));
	noStroke();

	// End with a light primary color rectangle
	fill(light_primary);
	rect(-10.0f, 31 * (height / 32.0f) + 1, (width / 2.0f) + 10.0f,
		height - (31 * (height / 32.0f)) + 1);

	// FPS Counter
	//fill(text_icons);
	//textSize(12.0f);
	//text("FPS: " + frameRate, 6.0f, 12.0f);
	popMatrix();
}
