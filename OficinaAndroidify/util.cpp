#include <ctime>
#include "util.h"
#include <OficinaFramework\OficinaFramework.hpp>
#include <OficinaFramework\OficinaExceptions.hpp>
#include <OficinaFramework\RenderingSystem.hpp>

static bool fillState     = true;
static bool strokeState   = true;
static Color4 fillColor   = Color4::MaskToColor4(WHITE);
static Color4 strokeColor = Color4::MaskToColor4(WHITE);

static OficinaFramework::RenderingSystem::Font* defaultFont;
static ProcessingProperties textAlignment[2] = { LEFT, BASELINE };
static float textMagnification = 1.0f;

float red(color c)
{
	return c.r;
}

float green(color c)
{
	return c.g;
}

float blue(color c)
{
	return c.b;
}

float alpha(color c)
{
	return c.a;
}

void noFill() {
	fillState = false;
}

void noStroke() {
	strokeState = false;
}

void background(int c) {
	background(c, c, c);
}

void background(color c) {
	background(c.r, c.g, c.b);
}

void background(int r, int g, int b)
{
	glClearColor(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, 0.0f);
}

void stroke(int c) {
	stroke(c, c, c, 255);
}

void stroke(color c) {
	stroke(c.r, c.g, c.b, 255);
}

void stroke(int r, int g, int b) {
	stroke(r, g, b, 255);
}

void stroke(int r, int g, int b, int opacity)
{
	strokeState = true;
	strokeColor.r = float(r) / 255.0f;
	strokeColor.g = float(g) / 255.0f;
	strokeColor.b = float(b) / 255.0f;
	strokeColor.a = float(opacity) / 255.0f;
}

void strokeWeight(int val) {
	glLineWidth(val);
}

void fill(int c) {
	fill(c, c, c, 255);
}

void fill(color c) {
	fill(c.r, c.g, c.b, 255);
}

void fill(int r, int g, int b) {
	fill(r, g, b, 255);
}

void fill(int r, int g, int b, int opacity)
{
	fillState = true;
	fillColor.r = float(r) / 255.0f;
	fillColor.g = float(g) / 255.0f;
	fillColor.b = float(b) / 255.0f;
	fillColor.a = float(opacity) / 255.0f;
}

void size(int w, int h) {
	OficinaFramework::ScreenSystem::SetWindowSize(vec2dw(w, h));
	OficinaFramework::RenderingSystem::SetResolution(vec2dw(w, h));
	if(!OficinaFramework::ScreenSystem::IsFullScreen())
		OficinaFramework::RenderingSystem::SetViewportSize(OficinaFramework::ScreenSystem::GetWindowSize());
}

void frameRate(int) {
	OF_Log(OF_LOG_LVL_WARNING, "frameRate: This method is deprecated.\n");
}

void rotate(float f) {
	glRotatef(radtodeg(f), 0.0f, 0.0f, 1.0f);
}

void translate(float x, float y) {
	glTranslatef(x, y, 1.0f);
}

void translate(vec2 v) {
	translate(v.x, v.y);
}

void scale(float x, float y) {
	glScalef(x, y, 1.0f);
}

void scale(vec2 v) {
	scale(v.x, v.y);
}

void pushMatrix() {
	glPushMatrix();
}

void popMatrix() {
	glPopMatrix();
}

void line(float x1, float y1, float x2, float y2)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void trueEllipse(float x, float y, float width, float height)
{
	pushMatrix();
		translate(x, y);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = degtorad(i);
			glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
		}
		glEnd();
	popMatrix();
}

void ellipse(float x, float y, float width, float height)
{
	if (fillState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
		trueEllipse(x, y, width, height);
	}
	if (strokeState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
		trueEllipse(x, y, width, height);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void trueArcStroke(float x, float y, float width, float height, float start, float stop, ProcessingProperties mode) {
	if (stop < start) return;

	pushMatrix();

		translate(x, y);
		int degInit = radtodeg(start);
		int degEnd = radtodeg(stop);

		glBegin((mode == OPEN ? GL_LINE_STRIP : GL_LINE_LOOP));
		for (int i = degInit; i < degEnd; i++)
		{
			float rad = degtorad(i);
			glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
		}

		// In case of a pie, we actually need to add a
		// final vertex.
		if (mode == PIE) glVertex2f(0.0f, 0.0f);

		glEnd();

	popMatrix();
}

void trueArc(float x, float y, float width, float height, float start, float stop, ProcessingProperties mode) {
	if (stop < start) return;

	pushMatrix();
	
		translate(x, y);
		int degInit = radtodeg(start);
		int degEnd = radtodeg(stop);
		switch (mode)
		{
		case OPEN:
		case CHORD: // Convex polygon
			glBegin(GL_POLYGON);
			for (int i = degInit; i < degEnd; i++)
			{
				float rad = degtorad(i);
				glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
			}
			glVertex2f(0.0f, 0.0f);
			glEnd();
			break;
		case PIE: // Concave polygon
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0.0f, 0.0f);
			for (int i = degInit; i < degEnd; i++)
			{
				float rad = degtorad(i);
				glVertex2f(cos(rad) * (width / 2.0f), sin(rad) * (height / 2.0f));
			}
			glEnd();
			break;
		default: break;
		}

	popMatrix();
}

void arc(float x, float y, float width, float height, float start, float stop, ProcessingProperties mode) {
	if ((stop < start) ||
		(mode != OPEN && mode != CHORD && mode != PIE)) return;
	if (fillState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
		trueArc(x, y, width, height, start, stop, mode);
	}
	if (strokeState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
		trueArcStroke(x, y, width, height, start, stop, mode);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void trueRect(float x, float y, float w, float h)
{
	pushMatrix();
		translate(x, y);
		glBegin(GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(w, 0.0f);
			glVertex2f(w, h);
			glVertex2f(0.0f, h);
		glEnd();
	popMatrix();
}

void rect(float x, float y, float w, float h)
{
	if (fillState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
		trueRect(x, y, w, h);
	}
	if (strokeState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
		trueRect(x, y, w, h);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void rect(float x, float y, float w, float h, float r)
{
	if (fillState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);

		// Internal rectangle
		trueRect(x + r, y + r, w - (2.0f * r), h - (2.0f * r));

		// Rectangles on borders
		// Top
		trueRect(x + r, y + (r / 2.0f), w - (2.0f * r), r / 2.0f);
		// Bottom
		trueRect(x + r, y + h - r, w - (2.0f * r), r / 2.0f);
		// Left
		trueRect(x + (r / 2.0f), y + r, r / 2.0f, h - (2.0f * r));
		// Right
		trueRect(x + w - r, y + r, r / 2.0f, h - (2.0f * r));
	}

	// Lines for the border rectangles
	if (strokeState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
		// Top
		line(x + r, y + (r / 2.0f), x + w - r, y + (r / 2.0f));
		// Bottom
		line(x + r, y - (r / 2.0f) + h, x + w - r, y - (r / 2.0f) + h);
		// Left
		line(x + (r / 2.0f), y + r, x + (r / 2.0f), y + h - r);
		// Right
		line(x - (r / 2.0f) + w, y + r, x - (r / 2.0f) + w, y + h - r);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Arcs
	// TopLeft
	arc(x + r, y + r, r, r, PI, 3 * HALF_PI, OPEN);
	// TopRight
	arc(x + (w - r), y + r, r, r, 3 * HALF_PI, TAU, OPEN);
	// BottomLeft
	arc(x + r, y + (h - r), r, r, HALF_PI, PI, OPEN);
	// BottomRight
	arc(x + (w - r), y + (h - r), r, r, 0, HALF_PI, OPEN);
}

void rect(float x, float y, float w, float h, float tl, float tr, float bl, float br)
{
	// Internal rectangles
	// we'll end up writing 5 rectangles in total, due to differences
	// on corner angles.
	if (fillState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
		// First, we draw rectangles related to radius sizes.
		// TopLeft->BottomLeft
		trueRect(x, y + (tl / 2.0f), fmax(tl, bl) / 2.0f, ceil(h - (bl / 2.0f) - (tl / 2.0f)) + 1.0f);
		// BottomLeft->BottomRight
		trueRect(x + (bl / 2.0f), y + (h - (fmax(bl, br) / 2.0f)), ceil(w - (bl / 2.0f) - (br / 2.0f)) + 1.0f, fmax(bl, br) / 2.0f);
		// BottomRight->TopRight
		trueRect(x + (w - fmax(br, bl) / 2.0f), y + (tr / 2.0f), fmax(br, bl) / 2.0f, ceil(h - (br / 2.0f) - (tr / 2.0f)) + 1.0f);
		// TopRight->TopLeft
		trueRect(x + (tl / 2.0f), y, ceil(w - (tr / 2.0f) - (tl / 2.0f)) + 1.0f, fmax(tr, tl));

		// Now we draw our internal rectangle.
		trueRect(x + fmax(tl, bl) / 2.0f, y + fmax(tl, tr) / 2.0f,
			(w - fmax(tl, bl) / 2.0f - fmax(tr, br) / 2.0f),
			(h - fmax(tl, tr) / 2.0f - fmax(bl, br) / 2.0f));
	}

	// Lines around borders
	// We'll need to draw them for each of our rectangles,
	// except for the internal rectangle.
	if (strokeState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
		// TopLeft->BottomLeft
		line(x, y + tl / 2.0f, x, y + h - bl / 2.0f);
		// BottomLeft->BottomRight
		line(x + bl / 2.0f, y + h, x + w - br / 2.0f, y + h);
		// BottomRight->TopRight
		line(x + w, y + h - br / 2.0f, x + w, y + tr / 2.0f);
		// TopRight->TopLeft
		line(x + tl / 2.0f, y, x + w - tr / 2.0f, y);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Arcs
	// TopLeft
	arc(x + tl / 2.0f, y + tl / 2.0f, tl, tl, PI, 3 * HALF_PI, OPEN);
	// TopRight
	arc(x + (w - tr / 2.0f), y + tr / 2.0f, tr, tr, 3 * HALF_PI, TAU, OPEN);
	// BottomLeft
	arc(x + bl / 2.0f, y + (h - bl / 2.0f), bl, bl, HALF_PI, PI, OPEN);
	// BottomRight
	arc(x + (w - br / 2.0f), y + (h - br / 2.0f), br, br, 0, HALF_PI, OPEN);
}

void trueTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	pushMatrix();
		glBegin(GL_TRIANGLES);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
		glEnd();
	popMatrix();
}

void triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	if (fillState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		OficinaFramework::RenderingSystem::glColorM(fillColor.GetMask(), fillColor.a);
		trueTriangle(x1, y1, x2, y2, x3, y3);
	}
	if (strokeState) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		OficinaFramework::RenderingSystem::glColorM(strokeColor.GetMask(), strokeColor.a);
		trueTriangle(x1, y1, x2, y2, x3, y3);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void textAlign(ProcessingProperties x) {
	textAlign(x, BASELINE);
}

void textAlign(ProcessingProperties x, ProcessingProperties y)
{
	if (x < LEFT || x > RIGHT ||
		y != CENTER && (y < TOP || y > BASELINE)) return;
	textAlignment[0] = x;
	textAlignment[1] = y;
}

void textSize(float size)
{
	textMagnification = size / float(defaultFont->GetCharacterSize().y);
}

void text(std::string str, float x, float y)
{
	vec2 measuredSize = defaultFont->MeasureString(str, textMagnification);
	switch (textAlignment[0]) {
	default: // Fall to LEFT
	case LEFT: break; // Nothing to do
	case RIGHT: x -= measuredSize.x; break;
	case CENTER: x -= (measuredSize.x / 2.0f); break;
	}

	switch (textAlignment[1]) {
	default: // Fall to BASELINE
	case BASELINE: // Which falls to BOTTOM
	case BOTTOM: y -= measuredSize.y; break;
	case CENTER: y -= (measuredSize.y / 2.0f); break;
	case TOP: break; // Nothing to do
	}

	// Now draw text
	defaultFont->DrawString(vec2(x, y), str, textMagnification, strokeColor, 1.0f);
}

float getDistance(float x1, float y1, float x2, float y2)
{
	return sqrtf(powf(x1 - x2, 2) + powf(y1 - y2, 2));
}

float random(float high)
{
	float power = 1.0f;

	while (truncf(high) != high) {
		power *= 10;
		high *= 10.0f;
	}

	return (rand() % static_cast<int>(high)) / power;
}

float random(float low, float high)
{
	return low + random(high - low);
}

Processing2DWindow::Processing2DWindow()
{
	SetActive(true);
	SetVisible(true);
}

void Processing2DWindow::Initialize()
{
	srand(time(nullptr));
	OficinaFramework::ScreenSystem::Screen::Initialize();
}

void Processing2DWindow::LoadContent() {
	width  = OficinaFramework::RenderingSystem::GetResolution().x;
	height = OficinaFramework::RenderingSystem::GetResolution().y;
	defaultFont = OficinaFramework::RenderingSystem::TexturePool::LoadDefaultFont();
	OficinaFramework::RenderingSystem::SetCameraPosition(vec2(300.0f, 0.0f));
	setup();
	OficinaFramework::ScreenSystem::Screen::LoadContent();
}

void Processing2DWindow::UnloadContent() {
	unload();
	OficinaFramework::ScreenSystem::Screen::UnloadContent();
}

void Processing2DWindow::Update() {
}
void Processing2DWindow::Draw() {
	OficinaFramework::RenderingSystem::SetCameraPosition(OficinaFramework::RenderingSystem::GetCameraPosition());
	pushMatrix();
	OficinaFramework::RenderingSystem::glTranslateToViewportPos();
	draw();
	popMatrix();
}
