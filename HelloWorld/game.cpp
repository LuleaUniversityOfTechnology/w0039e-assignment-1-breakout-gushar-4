#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"
#include "Paddle.h"
#include "game.h"
#include "Score.h"

bool isAlive = true;

Paddle paddle;


void SpawnBall(float ballSpeed, int ballX, int ballY)
{
	const int objectId = Play::CreateGameObject(ObjectType::TYPE_BALL, { DISPLAY_WIDTH / 2 + ballX , DISPLAY_HEIGHT / 2 + ballY }, 4, "ball");
	
	GameObject& ball = Play::GetGameObject(objectId);

	//float aRandomFloat = ((float)Play::RandomRollRange(-400, 400) / 100.0f);

	//ball.velocity = normalize({ aRandomFloat, 0 }) * ballSpeed;

	//ball.velocity = normalize({ aRandomFloat, -aRandomFloat }) * ballSpeed;

	//ball.velocity = normalize({ ((float)Play::RandomRollRange(-400, 400) / 100.0f) , -((float)Play::RandomRollRange(-400, 400) / 100.0f) }) * ballSpeed;

	//ball.velocity = normalize({ 0, -1 }) * ballSpeed;

	ball.velocity = normalize({ ((float)Play::RandomRollRange(-80, 80) / 100.0f), 1 }) * ballSpeed;

}

void SetupScene(int range)
{
	paddle = { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 10 };

	for (int i = -range; i < range + 1; i++)
	{

		for (int j = -range / 3; j < range / 3; j++)
		{
			Play::CreateGameObject(ObjectType::TYPE_BRICK, { DISPLAY_WIDTH / 2 + i * 20, DISPLAY_HEIGHT / 2  + j * 16 + DISPLAY_HEIGHT / 4 }, 10, "brick");
		}

	}

}


int Min(int x, int y)
{
	return x < y ? x : y;
}

int Max(int x, int y)
{
	return x > y ? x : y;
}

bool IsPaddleColliding(Play::GameObject& obj)
{
	Play::Point2D RectBottom = { paddle.x + 50, paddle.y + 5 };

	Play::Point2D RectTop = { paddle.x - 50, paddle.y - 5 };

	const float dx = obj.pos.x - Max(RectTop.x, Min(obj.pos.x, RectBottom.x));
	const float dy = obj.pos.y - Max(RectTop.y, Min(obj.pos.y, RectBottom.y));

	return (dx * dx + dy * dy) < (obj.radius * obj.radius);
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


void StepFrame(float elapsedTime)
{
	DrawScore(paddle);
	
	DrawPaddle(paddle);

	if (isAlive == true)
	{
		if (Play::KeyDown(Play::KEY_LEFT) && !(paddle.x - 60 < 0))
		{
			paddle.x = paddle.x - 5;
		}

		if (Play::KeyDown(Play::KEY_RIGHT) && !(paddle.x + 60 > DISPLAY_WIDTH))
		{
			paddle.x = paddle.x + 5;
		} 

		const std::vector<int> ballIds = Play::CollectGameObjectIDsByType(TYPE_BALL);

		const std::vector<int> BrickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);

		//balls
		for (int i = 0; i < ballIds.size(); i++)
		{

			Play::GameObject& object = Play::GetGameObject(ballIds[i]);

			if (object.pos.x > DISPLAY_WIDTH - 10 && object.velocity.x > 0) //right hand
			{

				object.velocity = randomNess(object.velocity, true, 40);

				object.velocity.x = -object.velocity.x;

			}

			else if (object.pos.x < 0 && object.velocity.x < 0) // left hand
			{

				object.velocity = randomNess(object.velocity, true, 40);

				object.velocity.x = -object.velocity.x;

			}

			if (object.pos.y > DISPLAY_HEIGHT - 10 && object.velocity.y > 0) // top
			{

				object.velocity = randomNess(object.velocity, false, 40);

				object.velocity.y = -object.velocity.y;

			}

			else if (object.pos.y < 0 && object.velocity.y < 0) // bottom
			{

				isAlive = false;
				UpdateScore();
				paddle = { DISPLAY_WIDTH / 2 ,DISPLAY_HEIGHT / 10 };
				object.pos = { DISPLAY_WIDTH / 2 ,DISPLAY_HEIGHT / 2 };
				int pitchSoundID = Play::PlayAudioPitch("error1", 20, 21);

				for (int c = 0; c < ballIds.size(); c++)
				{
					Play::DestroyGameObject(ballIds[c]);
				}

				for (int z = 0; z < BrickIds.size(); z++)
				{
					Play::DestroyGameObject(BrickIds[z]);
				}
				break;
			}

			if (IsPaddleColliding(object))
			{
				//int pitchSoundID = Play::PlayAudioPitch("error1", score + 50, score + 51);
				object.velocity.y = -object.velocity.y;
				object.pos.y = object.pos.y + 5;

			}

			Play::UpdateGameObject(object);

			Play::DrawObject(object);
		}

		//bricks
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

					if (ballObject.pos.x <= brickObject.pos.x && (ballObject.velocity.x <= 0) || ballObject.pos.x >= brickObject.pos.x && (ballObject.velocity.x >= 0))
					{
						//int pitchSoundID = Play::PlayAudioPitch("error1", score + 50, score + 51);
						ballObject.velocity.y = -ballObject.velocity.y;
						Play::DestroyGameObject(BrickIds[i]);
						ChangeScore(true);
					}

					else if (ballObject.pos.y <= brickObject.pos.y && (ballObject.velocity.y <= 0) || ballObject.pos.y >= brickObject.pos.y && (ballObject.velocity.y >= 0))
					{
						//int pitchSoundID = Play::PlayAudioPitch("error1", score + 50, score + 51);
						ballObject.velocity.x = -ballObject.velocity.x;
						Play::DestroyGameObject(BrickIds[i]);
						ChangeScore(true);
					}
				}
			}
		}
	}
	
	else
	{
		DeathScreen();
		DrawEndScore();

		if (isAlive == false && Play::KeyDown(KEY_ENTER))
		{
			ChangeScore(false);
			isAlive = true;
			SetupScene(15);
			SpawnBall(4, 0, -70);
			
		}

	}
}