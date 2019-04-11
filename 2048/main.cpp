#include "2048.h"
#include "AI.h"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <thread>

using namespace std;

void hello(int output) { cout << output << endl; }

int main () {

	thread test(hello, 2);
	test.join();

	int gameSize;
	int** board;
	bool gameLost;
	void getMove(int* move);
	
	srand((int)time(NULL));
	
	cout << "game size: " << endl;
	gameSize = 4;
	
	int totalScore = 0;
	int iterations = 1;
	
	for (int i = 0; i < iterations; i++) {
		gameLost = false;
		
		int score = 0;
		int freeTiles;
		
		Game2048::setupBoard(gameSize, &board, &score, &freeTiles);
		
		std::string input = "y";
		
		//cin >> input;
		
		if (input == "y") {
			AI ai(gameSize);
			while (!gameLost) {
				cout << "loop" << endl;
				int input;
				Game2048::searchDepth = 0;

				thread generatingMove(&AI::generateMove, &ai, &board, freeTiles, &input, 6);
				
				generatingMove.join();
				
				cout << "generated move" << endl;

				Game2048::executeMove(&board, gameSize, input, &score, &freeTiles, 1);
				Game2048::printBoard(&board, gameSize);
				if (freeTiles == 0) {
					gameLost = true;
					break;
				}
				Game2048::generateRandomTile(&board, gameSize, &score, &freeTiles);
				freeTiles -= 1;
				
			}
			totalScore += score;
		}
		else if (input == "n") {
			
			Game2048::printBoard(&board, gameSize);
			
			while (!gameLost) {
				int input;
				Game2048::getMove(&input);
				cout << "registered " << input << endl;
				Game2048::executeMove(&board, gameSize, input, &score, &freeTiles, 1);
				if (freeTiles == 0) {
					gameLost = true;
					break;
				}
				Game2048::generateRandomTile(&board, gameSize, &score, &freeTiles);
				freeTiles -= 1;
				Game2048::printBoard(&board, gameSize);
				cout << "Score: " << score << endl;
			}
		}
	}
	
	
	std::cout << "finished" << std::endl;
	std::cout << "avr score: " << totalScore / iterations << endl;

	return 0;
}
