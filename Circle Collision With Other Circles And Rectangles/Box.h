#pragma once

#include "Header.h"
#include "ConstGlobalVariables.h"

class Box
{
public:
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	float speed;

	float width;
	float height;
	Color color;

public:
	Box();
	Box(Vector2 position, float speed, float width, float height, Color color);

	void draw();
	void takeInput();
	void move();
	void checkBound();

	Vector2 getCenter();
};