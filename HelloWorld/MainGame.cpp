#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"
#include "game.h"
#include "paddle.h"
#include "Score.h"

using namespace Play;



// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);

	ScoreFileRead();
	SetupScene(15);
	SpawnBall(4, 0, -70);
	// 16248 limit


}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Play::ClearDrawingBuffer( Play::cBlack );

	StepFrame(elapsedTime);

	Play::PresentDrawingBuffer();
	return Play::KeyDown( KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	ScoreFileWrite();
	Play::DestroyManager();
	return PLAY_OK;
}
