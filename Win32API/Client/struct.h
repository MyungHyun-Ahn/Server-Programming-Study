#pragma once

struct Vec2
{
	float x;
	float y;


public:
	float Length()
	{
		return sqrt(x * x + y * y);
	}


	Vec2& Normalize()
	{
		float fLen = Length();

		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}

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

	Vec2 operator+(Vec2 vOther_)
	{
		return Vec2(x + vOther_.x, y + vOther_.y);
	}

	Vec2 operator-(Vec2 vOther_)
	{
		return Vec2(x - vOther_.x, y - vOther_.y);
	}

	Vec2 operator*(Vec2 vOther_)
	{
		return Vec2(x * vOther_.x, y * vOther_.y);
	}

	Vec2 operator/(Vec2 vOther_)
	{
		assert(!(vOther_.x == 0.f || vOther_.y == 0.f));
		return Vec2(x / vOther_.x, y / vOther_.y);
	}
};