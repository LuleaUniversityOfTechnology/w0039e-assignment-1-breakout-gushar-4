#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "paddle.h"

void SpawnBall(float ballSpeed, int ballX, int ballY);

void SetupScene(int range);

int Min(int x, int y);

int Max(int x, int y);

void StepFrame(float elapsedTime);

bool IsPaddleColliding(Play::GameObject& obj);

Play::Vector2D randomNess(Play::Vector2D obj, bool randomizeY, int Ness);