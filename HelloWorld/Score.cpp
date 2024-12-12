#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"
#include "paddle.h"
#include "game.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int score = 0;
int* highScores;

int scoreSize;

/*
void push_back(int& arr[], int x) {
	int newSize = ScoreSize + 1;

	int tempArr = new int[newSize];
	
	for (int i = 0; i < ScoreSize; i++)
	{
		tempArr[i] = newSize[i];
	}

	temp[newSize] = x;
	delete[] arr;
	arr = tempArr;
	delete[] tempArr;

}


myList = [1, 2, 3, 4, 5];
push_back(myList, 10);
myList = [1, 2, 3, 4, 5, 10];
*/

void DrawScore(Paddle paddle)
{
	std::string s = std::to_string(score);
	char const* pchar = s.c_str();

	Play::DrawDebugText({ paddle.x, paddle.y - 20 }, pchar, Play::cMagenta, true);
}


void ChangeScore(bool x)
{
	if (x)
	{
		score++;
	}
	else
	{
		score = 0;
	}
}


void UpdateScore()
{
	int newSize = scoreSize + 1;

	int* tempArr = new int[newSize];

	for (int i = 0; i < scoreSize; i++)
	{
		tempArr[i] = highScores[i];
	}

	delete[] highScores;

	for (int i = 0; i < newSize; i++)
	{
		if (score > tempArr[i])
		{
			for (int y = newSize - 1; y > i; y--)
			{
				tempArr[y] = tempArr[y - 1];
			}

			tempArr[i] = score;
			
			scoreSize = newSize;
			highScores = tempArr;


			break;
		}

	}

}


void DrawEndScore()
{
	int displayAmount = Min(scoreSize, 5);

	for (int i = 0; i < displayAmount; i++)
	{
		std::string s = std::to_string(highScores[i]);
		char const* pchar = s.c_str();
		Play::DrawDebugText({ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 + 50 - i * 20 }, pchar, Play::cWhite, true);
	}
}


void DeathScreen()
{

	std::string s = "u died";
	char const* deathmessage = s.c_str();
	Play::DrawDebugText({ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 3 }, deathmessage, Play::cRed, true);

	std::string e = "High Score";
	char const* highscore = e.c_str();
	Play::DrawDebugText({ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 + 80 }, highscore, Play::cRed, true);

}


void ScoreFileRead()
{

	ifstream scoreFile("Highscore.txt");
	if (scoreFile.is_open())
	{
		// read from file!
		string myText;
		
		int step = 0;
		
		while (getline(scoreFile, myText))
		{
			step++;
		}
		scoreSize = step;
		scoreFile.clear();
		scoreFile.seekg(0);

		highScores = new int[scoreSize];

		for (int i = 0; i < scoreSize; i++)
		{
			getline(scoreFile, myText);
			highScores[i] = stoi(myText);
		}

		scoreFile.close();
	}
	else
	{
		highScores = new int[scoreSize];
		// just fill array of 5 with 0s
	}
}

void ScoreFileWrite()
{
	ofstream scoreFile("Highscore.txt");

	for (int i = 0; i < scoreSize; i++)
	{
		scoreFile << highScores[i] << endl;
	}

	scoreFile.close();
}
