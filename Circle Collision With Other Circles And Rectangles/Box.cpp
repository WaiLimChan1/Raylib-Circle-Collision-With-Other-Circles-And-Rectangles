#include "Box.h"

Box::Box() {}
Box::Box(Vector2 position, float speed, float width, float height, Color color)
	: position(position), speed(speed), width(width), height(height), color(color)
{
	velocity = { 0,0 };
	acceleration = { 0,0 };
}

void Box::draw()
{
	DrawRectangle(position.x, position.y, width, height, BORDER_COLOR);
	DrawRectangle(position.x + BORDER_SIZE, position.y + BORDER_SIZE, width - BORDER_SIZE * 2, height - BORDER_SIZE * 2, color);
}
void Box::takeInput()
{

}
void Box::move()
{

}
void Box::checkBound()
{

}

Vector2 Box::getCenter()
{
	return { position.x + width / 2, position.y + height / 2 };
}