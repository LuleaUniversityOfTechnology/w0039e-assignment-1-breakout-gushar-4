#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

void SpawnBall(float ballSpeed, int ballX, int ballY);

void SetupScene(int range);

void StepFrame(float elapsedTime);

void DrawPaddle();

void DrawScore();

void UpdateScore();

void DrawEndScore();

void DeathScreen();

bool IsPaddleColliding(Play::GameObject& obj);

Play::Vector2D randomNess(Play::Vector2D obj, bool randomizeY, int Ness);