#include "Ball.h"

Ball::Ball() {}
Ball::Ball(Vector2 position, Vector2 velocity, float speed, float radius, Color color)
	: position(position), velocity(velocity), speed(speed), radius(radius), color(color)
{
	acceleration = { 0,0 };
	mass = PI * std::pow(radius, 2) * MASS_SCALE;
}

void Ball::draw()
{
	DrawCircle(position.x, position.y, radius, BORDER_COLOR);
	DrawCircle(position.x, position.y, radius - BORDER_SIZE, color);
}

void Ball::drawVelocity(Color color)
{
	DrawLine(position.x, position.y, position.x + velocity.x, position.y + velocity.y, color);
}

void Ball::takeInput()
{
	float dt = GetFrameTime();
	if (IsKeyDown(KEY_W)) position.y -= speed * dt;
	if (IsKeyDown(KEY_S)) position.y += speed * dt;
	if (IsKeyDown(KEY_A)) position.x -= speed * dt;
	if (IsKeyDown(KEY_D)) position.x += speed * dt;

	/*if (IsKeyDown(KEY_W)) velocity.y -= speed * dt;
	if (IsKeyDown(KEY_S)) velocity.y += speed * dt;
	if (IsKeyDown(KEY_A)) velocity.x -= speed * dt;
	if (IsKeyDown(KEY_D)) velocity.x += speed * dt;*/

	/*if (IsKeyDown(KEY_W)) acceleration.y -= speed * dt;
	if (IsKeyDown(KEY_S)) acceleration.y += speed * dt;
	if (IsKeyDown(KEY_A)) acceleration.x -= speed * dt;
	if (IsKeyDown(KEY_D)) acceleration.x += speed * dt;*/
}
void Ball::move()
{
	bool stop = false;
	if (IsKeyDown(KEY_W)) stop = true;
	if (IsKeyDown(KEY_S)) stop = true;
	if (IsKeyDown(KEY_A)) stop = true;
	if (IsKeyDown(KEY_D)) stop = true;

	if (!stop)
	{
		float dt = GetFrameTime();
		velocity.x += acceleration.x * dt;
		velocity.y += acceleration.y * dt;
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
	}
}

void Ball::checkBound()
{
	if (position.x - radius < 0) //Left Wall
	{
		position.x = radius;
		velocity.x *= -1;
		slowDown();
	}
	if (position.x + radius > SCREEN_WIDTH) //Right Wall
	{
		position.x = SCREEN_WIDTH - radius;
		velocity.x *= -1;
		slowDown();
	}
	if (position.y - radius < 0) //Top Wall
	{
		position.y = radius;
		velocity.y *= -1;
		slowDown();
	}
	if (position.y + radius > SCREEN_HEIGHT) //Bottom Wall
	{
		position.y = SCREEN_HEIGHT - radius;
		velocity.y *= -1;
		slowDown();
	}
}

//Change the direction of the ball to the targetPosition while preserving the speed
void Ball::changeDirectionTowards(Vector2 targetPosition)
{
	Vector2 direction = Vector2Normalize({ targetPosition.x - position.x, targetPosition.y - position.y });

	float speed = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
	if (speed < this->speed)
		speed = this->speed;

	velocity = Vector2Scale(direction, speed);
}

//Change the direction of the ball away from the targetPosition while preserving the speed
void Ball::changeDirectionAway(Vector2 targetPosition)
{
	Vector2 direction = Vector2Normalize({ position.x - targetPosition.x, position.y - targetPosition.y });

	float speed = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
	if (speed < this->speed)
		speed = this->speed;

	velocity = Vector2Scale(direction, speed);
}

//Calculate the closest point on the box.
Vector2 Ball::calculateClosestPoint(Box& box)
{
	Vector2 closestPoint = { 0,0 };

	if (position.x < box.position.x)
		closestPoint.x = box.position.x;
	else if (position.x > box.position.x + box.width)
		closestPoint.x = box.position.x + box.width;
	else
		closestPoint.x = position.x;

	if (position.y < box.position.y)
		closestPoint.y = box.position.y;
	else if (position.y > box.position.y + box.height)
		closestPoint.y = box.position.y + box.height;
	else
		closestPoint.y = position.y;

	return closestPoint;
}

//Calculate the distance between the center of the Ball and the closest point on the box.
float Ball::calculateDistance(Box& box)
{
	Vector2 closestPoint = calculateClosestPoint(box);
	float result = std::sqrt(std::pow(closestPoint.x - position.x, 2) + std::pow(closestPoint.y - position.y, 2));
	return result;
}

//Draw the line from the center of the Ball to the closest point on the box.
void Ball::drawLine(Box& box, Color color)
{
	Vector2 closestPoint = calculateClosestPoint(box);
	DrawLine(position.x, position.y, closestPoint.x, closestPoint.y, color);
}

//Handles collision with box and moves ball outward. 
void Ball::handleCollision(Box& box)
{
	Vector2 closestPoint = calculateClosestPoint(box);
	float distance = calculateDistance(box);

	//If there is a collision
	if (distance < radius)
	{
		Vector2 direction = Vector2Normalize({ position.x - closestPoint.x, position.y - closestPoint.y });

		//Move the ball outward
		float scale = radius - distance;
		Vector2 changeVector = Vector2Scale(direction, scale);

		position.x += changeVector.x;
		position.y += changeVector.y;

		//If ball is inside the box, move the ball outward away from the center of the box
		if (direction.x == 0 && direction.y == 0)
		{
			Vector2 boxCenter = box.getCenter();
			Vector2 directionFromCenter = Vector2Normalize({ position.x - boxCenter.x, position.y - boxCenter.y });
			float scale = radius;
			Vector2 changeVector = Vector2Scale(directionFromCenter, scale);
			position.x += changeVector.x;
			position.y += changeVector.y;
		}


		//Change the direction of the ball

		//Hitting the walls
		if (position.x != closestPoint.x) velocity.x *= -1;
		if (position.y != closestPoint.y) velocity.y *= -1;

		//Hitting the corners
		if ((closestPoint.x == box.position.x && closestPoint.y == box.position.y) ||
			(closestPoint.x == box.position.x + box.width && closestPoint.y == box.position.y) ||
			(closestPoint.x == box.position.x && closestPoint.y == box.position.y + box.height) ||
			(closestPoint.x == box.position.x + box.width && closestPoint.y == box.position.y + box.height))
		{
			float speed = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
			velocity.x = direction.x * speed;
			velocity.y = direction.y * speed;
		}

		slowDown();
	}
}

float Ball::calculateDistance(Ball& other)
{
	float result = std::sqrt(std::pow(other.position.x - this->position.x, 2) + std::pow(other.position.y - this->position.y, 2));
	return result;
}
void Ball::drawLine(Ball& other, Color color)
{
	DrawLine(this->position.x, this->position.y, other.position.x, other.position.y, color);
}

void Ball::slowDown()
{
	velocity.x *= COLLISION_PRESERVE;
	velocity.y *= COLLISION_PRESERVE;
}

//With Momentum
void Ball::handleCollision(Ball& other)
{
	float distance = calculateDistance(other);
	if (distance < radius + other.radius)
	{
		Vector2 direction = { this->position.x - other.position.x, this->position.y - other.position.y };
		direction = Vector2Normalize(direction);

		//Move the balls away from each other (Handling Static Collision)
		float offset = radius + other.radius - distance;
		position.x += direction.x * offset / 2;
		position.y += direction.y * offset / 2;

		other.position.x -= direction.x * offset / 2;
		other.position.y -= direction.y * offset / 2;

		//Change direction of the ball
		float thisInitialSpeed = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
		float otherInitialSpeed = std::sqrt(std::pow(other.velocity.x, 2) + std::pow(other.velocity.y, 2));
		float thisMass = mass;
		float otherMass = other.mass;
		float thisFinalSpeed = (thisMass - otherMass) * thisInitialSpeed / (thisMass + otherMass) + (2 * otherMass * otherInitialSpeed) / (thisMass + otherMass);
		float otherFinalSpeed = (otherMass - thisMass) * otherInitialSpeed / (thisMass + otherMass) + (2 * thisMass * thisInitialSpeed) / (thisMass + otherMass);
		velocity.x = direction.x * thisFinalSpeed;
		velocity.y = direction.y * thisFinalSpeed;

		other.velocity.x = -1 * direction.x * otherFinalSpeed;
		other.velocity.y = -1 * direction.y * otherFinalSpeed;
		
		slowDown();
	}
}

////No momentum
//void Ball::handleCollision(Ball& other)
//{
//	float distance = calculateDistance(other);
//	if (distance < radius + other.radius)
//	{
//		Vector2 direction = { this->position.x - other.position.x, this->position.y - other.position.y };
//		direction = Vector2Normalize(direction);
//
//		//Move the balls away from each other
//		float offset = radius + other.radius - distance;
//		position.x += direction.x * offset / 2;
//		position.y += direction.y * offset / 2;
//
//		other.position.x -= direction.x * offset / 2;
//		other.position.y -= direction.y * offset / 2;
//
//		//Change direction of the ball
//		float thisInitialSpeed = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
//		velocity.x = direction.x * thisInitialSpeed;
//		velocity.y = direction.y * thisInitialSpeed;
//
//		slowDown();
//	}
//}