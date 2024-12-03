#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"
#include "paddle.h"
#include "game.h"


void DrawPaddle(Paddle& paddle)
{

	Play::Point2D RectBottom = { paddle.x + 50, paddle.y + 5 };

	Play::Point2D RectTop = { paddle.x + -50, paddle.y + -5 };

	Play::DrawRect(RectTop, RectBottom, Play::cWhite, true);
}


