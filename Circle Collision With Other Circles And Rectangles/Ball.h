#pragma once

#include "Header.h"
#include "ConstGlobalVariables.h"
#include "Box.h"

class Ball
{
public:
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	float speed;

	float radius;
	float mass;

	Color color;

public:
	Ball();
	Ball(Vector2 position, Vector2 velocity, float speed, float radius, Color color);

	void draw();
	void drawVelocity(Color color);

	void takeInput();

	void move();
	void checkBound();

	void changeDirectionTowards(Vector2 targetPosition);
	void changeDirectionAway(Vector2 targetPosition);

	Vector2 calculateClosestPoint(Box& box);
	float calculateDistance(Box& box);
	void drawLine(Box& box, Color color);
	void handleCollision(Box& box);

	float calculateDistance(Ball& other);
	void drawLine(Ball& other, Color color);
	void handleCollision(Ball& other);

	void slowDown();
};