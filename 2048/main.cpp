#include "2048.h"
#include "AI.h"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

#ifdef _WIN64
#define WINPAUSE system("pause")
#endif

using namespace std;

int main () {

	
	
	int gameSize;
	int** board;
	bool lkey;
	bool rkey;
	bool ukey;
	bool dkey;
	bool gameLost;
	void getMove(int* move);
	
	srand((int)time(NULL));
	
	cout << "game size: " << endl;
	cin >> gameSize;
	
	ukey = false;
	dkey = false;
	rkey = false;
	lkey = false;
	gameLost = false;
	
	int score = 0;
	int freeTiles;
	
	Game2048::setupBoard(gameSize, &board, &score, &freeTiles);
	
	cout << "Utilize AI? (Y/N)" << endl;
	
	std::string input = "y";
	
	//cin >> input;
	
	if (input == "y") {
		AI ai(gameSize);
		while (!gameLost) {
			cout << "Starting round.." << endl;
			int input;
			Game2048::searchDepth = 0;
			cout << "Generating move.." << endl;
			input = ai.generateMove(&board, freeTiles);
			cout << "Generated move. Max search depth was: " << Game2048::searchDepth << endl;
			Game2048::executeMove(&board, gameSize, input, &score, &freeTiles, 1);
			cout << "Executed move. Freetiles: " << freeTiles << endl;
			if (freeTiles == 0) {
				gameLost = true;
				break;
			}
			cout << "Generating tile" << endl;
			Game2048::generateRandomTile(&board, gameSize, &score, &freeTiles);
			freeTiles -= 1;
			cout << "printing board.. " << endl;
			Game2048::printBoard(&board, gameSize);
			//std::this_thread::sleep_for(std::chrono::milliseconds(500));
			
		}
	}
	else if (input == "n") {
		exit(1);
		
		Game2048::printBoard(&board, gameSize);
		
		while (!gameLost) {
			int input;
			//Game2048::getMove(&input);
			Game2048::executeMove(&board, gameSize, input, &score, &freeTiles, 0);
			if (freeTiles == 0) {
				gameLost = true;
				break;
			}
			Game2048::generateRandomTile(&board, gameSize, &score, &freeTiles);
			freeTiles -= 1;
			Game2048::printBoard(&board, gameSize);
		}
	}
	else { cout << "Invalid input." << endl; }
	
	std::cout << "finished" << std::endl;

	return 0;
}
