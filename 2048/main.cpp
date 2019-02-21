#include "2048.h"
#include "AI.h"
#include <iostream>

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
	
	std::string input;
	
	cin >> input;
	
	if (input == "y") {
		AI ai(gameSize);
		while (!gameLost) {
			cout << "Starting round.." << endl;
			int input;
			cout << "Generating move.." << endl;
			input = ai.generateMove(&board);
			cout << "Generated move" << endl;
			Game2048::executeMove(&board, gameSize, input, &score, &freeTiles);
			if (freeTiles == 0) {
				gameLost = true;
				break;
			}
			Game2048::generateRandomTile(&board, gameSize, &score, &freeTiles);
			freeTiles -= 1;
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
			Game2048::executeMove(&board, gameSize, input, &score, &freeTiles);
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
	
	
	return 0;
}
