#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cmath>
#include "2048.h"

int Game2048::searchDepth = 0;

using namespace std;

Game2048::Game2048(int size) {
}

/*
void Game2048::getMove(int* move) {
	bool foundkey = false;
	while (foundkey == false) {
		
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			if (!rkey) {
				*move = 1;
				foundkey = true;
			}
			rkey = true;
		}
		else rkey = false;

		if (GetKeyState(VK_LEFT) & 0x8000) {
			if (!lkey) {
				*move = 2;
				foundkey = true;
			}
			lkey = true;
		}
		else lkey = false;

		if (GetKeyState(VK_DOWN) & 0x8000) {
			if (!dkey) {
				*move = 3;
				foundkey = true;
			}
			dkey = true;
		}
		else dkey = false;

		if (GetKeyState(VK_UP) & 0x8000) {
			if (!ukey) {
				*move = 4;
				foundkey = true;
			}
			ukey = true;
		}
		else ukey = false;
	}
}
*/

void Game2048::getMove(int *move) {
	(*move) = 1;
}

void Game2048::setupBoard(int size, int*** board, int* score, int* freeTiles) {
	*board = new int*[size];
	for (int i = 0; i < size; i++) {
		(*board)[i] = new int[size];
		for (int j = 0; j < size; j++) {
			(*board)[i][j] = 0;
		}
	}
	Game2048::generateRandomTile(board, size, score, freeTiles);

	(*freeTiles) = size * size - 1;

}

void Game2048::generateRandomTile(int*** board, int size, int* score, int* freeTiles) {
	bool finished = false;

	int n;
	cout << "Starting board:" << endl;
	Game2048::printBoard(board, size);
	cin;
	while (!finished) {
		n = rand() % (size*size);
		//cout << "x: " << n%4 << "y:" << (int)floor((double)n / 4.) << endl;
		if ((*board)[n % 4][(int)floor((double)n / 4.)] == 0) {
			(*board)[n % 4][(int)floor((double)n / 4.)] = rand() % 100 < 10 ? 4 : 2;
			finished = true;
		}
	}
	cout << "Ending board: " << endl;
	Game2048::printBoard(board, size);

	
}

void Game2048::printBoard(int*** board, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << (*board)[i][j] << ((j == size - 1) ? "" : " : ");
		}
		cout << endl;
	}
	cout << endl;
}

void Game2048::generateRemainingMovesFromBoard(int*** board, int size, int turn, int* freeTiles, std::vector<int>* remainingMoves) {
	(*remainingMoves).clear();
	if (turn % 2 == 0) {
		(*remainingMoves).push_back(1);
		(*remainingMoves).push_back(2);
		(*remainingMoves).push_back(3);
		(*remainingMoves).push_back(4);
	}
	(*freeTiles) = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((*board)[i][j] == 0) {
				(*freeTiles)++;
				if (turn % 2 == 1) {
					(*remainingMoves).push_back(i + 4 * j);
					(*remainingMoves).push_back(i + 4 * j + 16);
				}
			}
		}
	}
}

void Game2048::executeMove(int*** board, int size, int move, int* score, int* freeTiles, int turn) {
	if (turn % 2 == 1) {
		switch (move)
		{
		case 1:
			rotateCW(board, size);
			rotateCW(board, size);
			moveLeft(board, size, score, freeTiles);
			rotateCW(board, size);
			rotateCW(board, size);
			break;
		case 2:
			moveLeft(board, size, score, freeTiles);
			break;
		case 3:
			rotateCCW(board, size);
			moveLeft(board, size, score, freeTiles);
			rotateCW(board, size);
			break;
		case 4:
			rotateCW(board, size);
			moveLeft(board, size, score, freeTiles);
			rotateCCW(board, size);
			break;
		default:
			break;
		}
	}
	else {
		if (move < 0 || move >= 32) exit(19);

		int tile = move < 16 ? 2 : 4;
		move = move % 16;
		(*board)[move % 4][int(floor(move / 4))] = tile;
	}
}

void Game2048::moveLeft(int*** board, int size, int* score, int* freeTiles) {
	int index;
	for (int i = 0; i < size; i++) {
		index = 0;
		for (int j = 0; j < size; j++) {
			if ((*board)[i][j] != 0) {
				if (index - 1 >= 0 && (*board)[i][j] == (*board)[i][index - 1]) {
					*score = *score + (*board)[i][index - 1] * 2;
					(*board)[i][index - 1] *= 2;
					(*board)[i][j] = 0;
					(*freeTiles)++;
					index--;
				}
				else if (index != j) {
					(*board)[i][index] = (*board)[i][j];
					(*board)[i][j] = 0;
					
				}
				index++;
			}
		}
	}
}

void Game2048::rotateCW(int*** board, int size) {
	int** tmpBoard = new int*[size];
	for (int i = 0; i < size; i++) {
		tmpBoard[i] = new int[size];
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			tmpBoard[i][j] = (*board)[i][j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			(*board)[i][j] = tmpBoard[j][size - 1 - i];
		}
	}
}

void Game2048::rotateCCW(int*** board, int size) {
	int** tmpBoard = new int*[size];
	for (int i = 0; i < size; i++) {
		tmpBoard[i] = new int[size];
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			tmpBoard[i][j] = (*board)[i][j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			(*board)[j][size - 1 - i] = tmpBoard[i][j];
		}
	}
}
