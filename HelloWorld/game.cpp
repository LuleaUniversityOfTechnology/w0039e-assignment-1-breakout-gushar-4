#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"
#include "game.h"


void SpawnBall(float ballSpeed, int ballX, int ballY)
{
	const int objectId = Play::CreateGameObject(ObjectType::TYPE_BALL, { DISPLAY_WIDTH / 2 + ballX , DISPLAY_HEIGHT / 2 + ballY }, 4, "ball");
	
	GameObject& ball = Play::GetGameObject(objectId);

	//float aRandomFloat = ((float)Play::RandomRollRange(-400, 400) / 100.0f);

	//ball.velocity = normalize({ aRandomFloat, 0 }) * ballSpeed;

	//ball.velocity = normalize({ aRandomFloat, -aRandomFloat }) * ballSpeed;

	//ball.velocity = normalize({ ((float)Play::RandomRollRange(-400, 400) / 100.0f) , -((float)Play::RandomRollRange(-400, 400) / 100.0f) }) * ballSpeed;

	//ball.velocity = normalize({ 0, -1 }) * ballSpeed;

	ball.velocity = normalize({ ((float)Play::RandomRollRange(-400, 400) / 100.0f), 1 }) * ballSpeed;

}

void SetupScene(int range)
{
	//Play::CreateGameObject(ObjectType::TYPE_BRICK, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 6, "brick");
	//Play::CreateGameObject(ObjectType::TYPE_BRICK, { DISPLAY_WIDTH / 2 + 20, DISPLAY_HEIGHT / 2 }, 6, "brick");

	for (int i = -range; i < range; i++)
	{

		for (int j = -range / 3; j < range / 3; j++)
		{
			Play::CreateGameObject(ObjectType::TYPE_BRICK, { DISPLAY_WIDTH / 2 + i * 20, DISPLAY_HEIGHT / 2  + j * 16 + DISPLAY_HEIGHT / 4 }, 10, "brick");
		}

	}

}


Play::Vector2D randomNess(Play::Vector2D obj, bool randomizeY, int Ness)
{
	Play::Vector2D V = obj;
	float m = V.Length();
	V.Normalize();
	float randomFloat = ((float)Play::RandomRollRange(-Ness, Ness) / 100.0f);
	V += { randomFloat * (1 - randomizeY), randomFloat * randomizeY};
	V.Normalize();
	V *= m;

	return V;
}

//Play::Vector2D randomNess2(Play::Vector2D obj)
//{
//	Play::Vector2D V = obj;
//	float m = V.Length();
//	V.Normalize();
//	float randomX = Play::RandomRollRange(400, 600) / 1000.0f;
//	float randomY = Play::RandomRollRange(400, 600) / 1000.0f;
//	//float randomY = 1 - randomX;
//	Play::Vector2D Vout{ randomX, randomY };
//	
//	Vout.Normalize();
//
//	Vout *= m;
//
//	return Vout;
//}

void StepFrame(float elapsedTime)
{
	const std::vector<int> ballIds = Play::CollectGameObjectIDsByType(TYPE_BALL);

	const std::vector<int> BrickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);
	
	for (int i = 0; i < ballIds.size(); i++)
	{
		
		Play::GameObject& object = Play::GetGameObject(ballIds[i]);

		if (object.pos.x > DISPLAY_WIDTH -10 && object.velocity.x > 0) //right hand
		{

			object.velocity = randomNess(object.velocity, true, 40);

			object.velocity.x = -object.velocity.x;

		}

		else if (object.pos.x < 0 && object.velocity.x < 0) // left hand
		{		

			object.velocity = randomNess(object.velocity, true, 40);

			object.velocity.x = -object.velocity.x;

		}

		if (object.pos.y > DISPLAY_HEIGHT -10 && object.velocity.y > 0) // top
		{

			object.velocity = randomNess(object.velocity, false, 40);

			object.velocity.y = -object.velocity.y;
		}

		else if (object.pos.y < 0 && object.velocity.y < 0) // bottom
		{

			object.velocity = randomNess(object.velocity, false, 40);

			object.velocity.y = -object.velocity.y;

		}

		Play::UpdateGameObject(object);
		
		Play::DrawObject(object);
	}
	
	for (int i = 0; i < BrickIds.size(); i++)
	{
		Play::GameObject& brickObject = Play::GetGameObject(BrickIds[i]);
		Play::DrawObject(brickObject);
		Play::CentreAllSpriteOrigins();

		for (int y = 0; y < ballIds.size(); y++)
		{
			Play::GameObject& ballObject = Play::GetGameObject(ballIds[y]);

			if (Play::IsColliding(Play::GetGameObject(BrickIds[i]), Play::GetGameObject(ballIds[y])))
			{
				Play::UpdateGameObject(brickObject);

				if (ballObject.pos.x < brickObject.pos.x && (ballObject.velocity.x < 0) || ballObject.pos.x > brickObject.pos.x && (ballObject.velocity.x > 0))
				{
					ballObject.velocity.y = -ballObject.velocity.y;
					Play::DestroyGameObject(BrickIds[i]);
				}

				else if (ballObject.pos.y < brickObject.pos.y && (ballObject.velocity.y < 0) || ballObject.pos.y > brickObject.pos.y && (ballObject.velocity.y > 0))
				{
					ballObject.velocity.x = -ballObject.velocity.x;
					Play::DestroyGameObject(BrickIds[i]);
				}

			}

		}
		
		
	}
}