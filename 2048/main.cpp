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
	gameSize = 4;
	
	int totalScore = 0;
	int iterations = 2;
	
	for (int i = 0; i < iterations; i++) {
		
		ukey = false;
		dkey = false;
		rkey = false;
		lkey = false;
		gameLost = false;
		
		int score = 0;
		int freeTiles;
		
		Game2048::setupBoard(gameSize, &board, &score, &freeTiles);
		
		std::string input = "y";
		
		//cin >> input;
		
		if (input == "y") {
			AI ai(gameSize);
			while (!gameLost) {
				int input;
				Game2048::searchDepth = 0;
				input = ai.generateMove(&board, freeTiles);
				Game2048::executeMove(&board, gameSize, input, &score, &freeTiles, 1);
				if (freeTiles == 0) {
					gameLost = true;
					break;
				}
				Game2048::generateRandomTile(&board, gameSize, &score, &freeTiles);
				freeTiles -= 1;
				Game2048::printBoard(&board, gameSize);
				//std::this_thread::sleep_for(std::chrono::milliseconds(500));
				
			}
			totalScore += score;
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
				//Game2048::printBoard(&board, gameSize);
			}
		}
	}
	
	
	std::cout << "finished" << std::endl;
	std::cout << "avr score: " << totalScore / iterations << endl;

	return 0;
}
