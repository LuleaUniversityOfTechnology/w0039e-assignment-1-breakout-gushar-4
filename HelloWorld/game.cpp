#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"
#include "game.h"


void SpawnBall(float ballSpeed, int ballX, int ballY)
{
	const int objectId = Play::CreateGameObject(ObjectType::TYPE_BALL, { DISPLAY_WIDTH / 2 + ballX , DISPLAY_HEIGHT / 2 + ballY }, 4, "ball");
	
	GameObject& ball = Play::GetGameObject(objectId);
	ball.velocity = normalize({ 0, -1 }) * ballSpeed;

}

Play::Vector2D randomNess(Play::Vector2D obj)
{
	Play::Vector2D V = obj;
	float m = V.Length();
	V.Normalize();
	V += { 0, Play::RandomRollRange(-1, 1) };
	V.Normalize();
	V *= m;

	return V;
}

Play::Vector2D randomNess2(Play::Vector2D obj)
{
	Play::Vector2D V = obj;
	float m = V.Length();
	V.Normalize();
	float randomX = Play::RandomRollRange(4, 6) / 10.0f;
	float randomY = 1 - randomX;
	//float randomY = Play::RandomRollRange(0.2, 0.8);
	Play::Vector2D Vout{ randomX, randomY };
	
	Vout.Normalize();

	Vout *= m;

	return Vout;
}

void StepFrame(float elapsedTime)
{
	const std::vector<int> ballIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	
	for (int i = 0; i < ballIds.size(); i++)
	{
		
		Play::GameObject& object = Play::GetGameObject(ballIds[i]);

		if (object.pos.x > DISPLAY_WIDTH -10 && object.velocity.x > 0) //right hand
		{
			Play::Vector2D newVector = randomNess2(object.velocity);
			
			object.velocity.x = -newVector.x;

			object.velocity.y = newVector.y;
		}

		else if (object.pos.x < 0 && object.velocity.x < 0) // left hand
		{
			Play::Vector2D newVector = randomNess2(object.velocity);
			
			object.velocity.x = newVector.x;

			object.velocity.y = -newVector.y;

		}

		if (object.pos.y > DISPLAY_HEIGHT -10 && object.velocity.y > 0) // top
		{
			Play::Vector2D newVector = randomNess2(object.velocity);

			object.velocity.x = -newVector.x;

			object.velocity.y = -newVector.y;
		}

		else if (object.pos.y < 0 && object.velocity.y < 0) // bottom
		{
			Play::Vector2D newVector = randomNess2(object.velocity);

			object.velocity.x = -newVector.x;

			object.velocity.y = newVector.y;

		}

		Play::UpdateGameObject(object);
		
		Play::DrawObject(object);
	}
}