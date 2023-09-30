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

		assert(fLen != 0.f); // 0이 아니면 assert

		// 길이로 나눠줌 0으로 나누게 되면? -> 오류
		// 정규화를 거치면 1보다 작았던 것들도 1로 맞춰짐
		// 0, 0 은 말이 안된다.
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
};