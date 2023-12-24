#include "Header.h"
#include "ConstGlobalVariables.h"

#include "Box.h"
#include "Ball.h"

//Global Variables-----------------------------------------------------------
std::vector<Ball> balls;
std::vector<Box> boxes;

bool toggleBoxCollisionLines;
bool toggleBallCollisionLines;
bool toggleVelocityLines;
//---------------------------------------------------------------------------

void AddRandomBall()
{
	Ball ball;

	Vector2 ballPos = { (float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT) };
	Vector2 ballVel = { (float)(rand() % 600 - 300), (float)(rand() % 600 - 300) };
	float ballSpeed = rand() % 300 + 500;
	float ballRadius = rand() % 50 + 10;
	//float ballRadius = 30;
	Color ballColor = { rand() % 255, rand() % 255, rand() % 255, 255 };
	ball = Ball(ballPos, ballVel, ballSpeed, ballRadius, ballColor);

	ball.acceleration.y = GRAVITY;

	balls.push_back(ball);
}

void AddRandomBox()
{
	Box box;

	Vector2 boxPos = { (float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT) };
	float boxWidth = rand() % 200 + 50;
	float boxHeight = rand() % 200 + 50;
	Color boxColor = { rand() % 255, rand() % 255, rand() % 255, 255 };
	box = Box(boxPos, 300, boxWidth, boxHeight, boxColor);

	boxes.push_back(box);
}

void SetUp()
{
	toggleBoxCollisionLines = false;
	toggleBallCollisionLines = false;
	toggleVelocityLines = false;

	//Generate balls
	for (int i = 0; i < 10; i++)
		AddRandomBall();

	//Generate boxes
	for (int i = 0; i < 0; i++)
		AddRandomBox();
}

void PrintText(Ball ball, Box box)
{
	char message[50];
	Vector2 textPos{ 10, 10 };
	int fontSize = 30;
	Color textColor = YELLOW;

	//Position of the circle
	strcpy_s(message, "Circle Position: (");
	strcat_s(message, std::to_string(static_cast<int>(ball.position.x)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, std::to_string(static_cast<int>(ball.position.y)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPos.x, textPos.y, fontSize, textColor);
	textPos.y += fontSize;

	//Closest Point
	Vector2 closestPoint = ball.calculateClosestPoint(box);
	strcpy_s(message, "Closet Point Position: (");
	strcat_s(message, std::to_string(static_cast<int>(closestPoint.x)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, std::to_string(static_cast<int>(closestPoint.y)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPos.x, textPos.y, fontSize, textColor);
	textPos.y += fontSize;

	//Distance
	float distance = ball.calculateDistance(box);
	strcpy_s(message, "Distance: ");
	strcat_s(message, std::to_string(static_cast<int>(distance)).c_str());
	DrawText(message, textPos.x, textPos.y, fontSize, textColor);
}

void Draw()
{
	BeginDrawing();

	ClearBackground(BLUE);

	for (Box& box : boxes)
		box.draw();

	for (Ball& ball : balls)
	{
		ball.draw();
	}

	if (toggleBoxCollisionLines)
		for (Ball& ball : balls)
			for (Box& box : boxes)
				ball.drawLine(box, ORANGE);

	if (toggleBallCollisionLines)
		for (int i = 0; i < balls.size() - 1; i++)
			for (int j = i + 1; j < balls.size(); j++)
				balls.at(i).drawLine(balls.at(j), GREEN);

	if (toggleVelocityLines)
		for (Ball& ball : balls)
			ball.drawVelocity(PURPLE);

	//PrintText(balls.at(0), boxes.at(0));

	EndDrawing();
}

void Input()
{
	for (Ball& ball : balls)
	{
		ball.takeInput();
	}

	//Reset the simulation
	if (IsKeyPressed(KEY_R))
	{
		balls.clear();
		boxes.clear();
		SetUp();
	}

	//When left mouse button is held, all of the balls move towards the mouse cursor.
	if (IsMouseButtonDown(0))
	{
		for (Ball& ball : balls)
		{
			ball.changeDirectionTowards({ (float) GetMouseX(), (float) GetMouseY() });
		}
	}

	//When right mouse button is held, all of the balls move away from the mouse cursor.
	if (IsMouseButtonDown(1))
	{
		for (Ball& ball : balls)
		{
			ball.changeDirectionAway({ (float)GetMouseX(), (float)GetMouseY() });
		}
	}
	
	//Toggle drawing the collision lines and velocity lines
	if (IsKeyPressed(KEY_F)) toggleBallCollisionLines = !toggleBallCollisionLines;
	if (IsKeyPressed(KEY_G)) toggleBoxCollisionLines = !toggleBoxCollisionLines;
	if (IsKeyPressed(KEY_H)) toggleVelocityLines = !toggleVelocityLines;


	//Change the velocity of all the balls with the maximum speed in x and y direction being given by MAX_POSSIBLE_SPEED
	if (IsKeyPressed(KEY_T))
	{
		for (Ball& ball : balls)
		{
			float max = MAX_POSSIBLE_RANDOM_SPEED;
			ball.velocity = { (float)(rand() % (int) max - max/2), (float)(rand() % (int) max - max/2) };
		}
	}

	if (IsKeyPressed(KEY_Y))
	{
		AddRandomBall();
	}

	if (IsKeyPressed(KEY_U))
	{
		AddRandomBox();
	}
}

void Logic()
{
	for (Ball& ball : balls)
	{
		ball.move();

		for (Box& box : boxes)
			ball.handleCollision(box);

		for (Ball& otherBall : balls)
			if (&ball != &otherBall)
				ball.handleCollision(otherBall);
		ball.checkBound();
	}
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Circle VS Rectangle Collision Explaination");
	SetWindowState(FLAG_VSYNC_HINT);

	srand(time(0));
	SetUp();
	while (!WindowShouldClose())
	{
		Draw();
		Input();
		Logic();
	}
	return 0;
}