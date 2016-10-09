#pragma once
#include <OficinaFramework/ScreenSystem.hpp>

enum ProcessingProperties
{
	OPEN,
	CHORD,
	PIE,
	LEFT,
	CENTER,
	RIGHT,
	TOP,
	BOTTOM,
	BASELINE
};

static constexpr float PI         = M_PI;
static constexpr float TAU        = M_PI * 2.0f;
static constexpr float TWO_PI     = M_PI * 2.0f;
static constexpr float HALF_PI    = M_PI / 2.0f;
static constexpr float QUARTER_PI = M_PI / 4.0f;

struct color
{
	int r, g, b, a;

	color(int red, int green, int blue, int alpha)
		: r(red), g(green), b(blue), a(alpha) {}
	color(int num) : color(num, num, num, 255) {}
	color(int r, int g, int b) : color(r, g, b, 255) {}
	color() : color(0) {}
};
float red(color);
float green(color);
float blue(color);
float alpha(color);


void noFill();
void noStroke();
void background(int);
void background(color);
void background(int, int, int);
void stroke(int);
void stroke(color);
void stroke(int, int, int);
void stroke(int, int, int, int);
void strokeWeight(int);
void fill(int);
void fill(color);
void fill(int, int, int);
void fill(int, int, int, int);
void size(int, int);
void frameRate(int);

void rotate(float);
void translate(float, float);
void translate(vec2);
void scale(float, float);
void scale(vec2);
void pushMatrix();
void popMatrix();

void line(float, float, float, float);
void ellipse(float, float, float, float);
void arc(float, float, float, float, float, float, ProcessingProperties mode = OPEN);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float r);
void rect(float x, float y, float w, float h, float tl, float tr, float bl, float br);
void triangle(float x1, float y1, float x2, float y2, float x3, float y3);

void textAlign(ProcessingProperties);
void textAlign(ProcessingProperties, ProcessingProperties);
void textSize(float);
void text(std::string, float, float);

float getDistance(float, float, float, float);

float random(float high);
float random(float low, float high);

class Processing2DWindow : public OficinaFramework::ScreenSystem::Screen
{
protected:
	int width, height;
public:
	Processing2DWindow();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void setup()  = 0;
	virtual void unload() = 0;
	virtual void draw()   = 0;
};