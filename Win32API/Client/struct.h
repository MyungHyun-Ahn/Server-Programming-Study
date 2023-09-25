#pragma once

struct Vec2
{
	float x;
	float y;

public:
	Vec2() : x(0.f), y(0.f) {}
	Vec2(float x_, float y_) : x(x_), y(y_) {}

	Vec2(int x_, int y_) : x((float)x_), y((float)y_) {}

	Vec2(POINT pt_) : x((float)pt_.x), y((float)pt_.y) {}

	Vec2& operator=(POINT pt_)
	{
		x = (float)pt_.x;
		y = (float)pt_.y;
	}
};