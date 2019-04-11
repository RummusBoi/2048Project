#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cmath>
#include "2048.h"
#ifndef __APPLE__
	#include <Windows.h>
#define OPERATINGSYSTEM 1
#endif
int Game2048::searchDepth = 0;
bool Game2048::lkey = false;
bool Game2048::rkey = false;
bool Game2048::ukey = false;
bool Game2048::dkey = false;


using namespace std;


Game2048::Game2048(int size) {
}

#ifndef __APPLE__
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

		if (GetKeyState(VK_UP) & 0x8000) {
			if (!dkey) {
				*move = 3;
				foundkey = true;
			}
			dkey = true;
		}
		else dkey = false;

		if (GetKeyState(VK_DOWN) & 0x8000) {
			if (!ukey) {
				*move = 4;
				foundkey = true;
			}
			ukey = true;
		}
		else ukey = false;
	}
}
#endif
#ifdef __APPLE__
void Game2048::getMove(int* move) {}
#endif	
char Game2048::charFromMove(int move) {
	switch (move) {
		case 1:
			return 'r';
		case 2: 
			return 'l';
		case 3:
			return 'u';
		case 4:
			return 'd';
		default:
			exit(-1);
	}
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
	while (!finished) {
		n = rand() % (size*size);
		//cout << "x: " << n%4 << "y:" << (int)floor((double)n / 4.) << endl;
		if ((*board)[n % size][(int)floor((double)n / size)] == 0) {
			(*board)[n % size][(int)floor((double)n / size)] = rand() % 100 < 10 ? 4 : 2;
			finished = true;
		}
	}

	
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
					(*remainingMoves).push_back(i + size * j);
					(*remainingMoves).push_back(i + size * j + size*size);
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
			moveRight(board, size, score, freeTiles);
			break;
		case 2:
			moveLeft(board, size, score, freeTiles);
			break;
		case 3:
			moveUp(board, size, score, freeTiles);
			break;
		case 4:
			moveDown(board, size, score, freeTiles);
			break;
		default:
			break;
		}
	}
	else {
		if (move < 0 || move > 2*size*size) {cout << move; throw new exception();}

		int tile = move < size*size ? 2 : 4;
		move = move % (size*size);
		(*board)[move % size][int(floor(move / size))] = tile;
		(*freeTiles) -= 1;
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

void Game2048::moveUp(int*** board, int size, int* score, int* freeTiles) {
	int index;
	for (int j = 0; j < size; j++) {
		index = 0;
		for (int i = 0; i < size; i++) {
			if ((*board)[i][j] != 0) {
				if (index - 1 >= 0 && (*board)[i][j] == (*board)[index - 1][j]) {
					*score = *score + (*board)[index - 1][j] * 2;
					(*board)[index - 1][j] *= 2;
					(*board)[i][j] = 0;
					(*freeTiles)++;
					index--;
				}
				else if (index != i) {
					(*board)[index][j] = (*board)[i][j];
					(*board)[i][j] = 0;
					
				}
				index++;
			}
		}
	}
}

void Game2048::moveRight(int*** board, int size, int *score, int *freeTiles) {
	int index;
	for (int i = size - 1; i >= 0; i--) {
		index = size - 1;
		for (int j = size - 1; j >= 0; j--) {
			if ((*board)[i][j] != 0) {
				if (index + 1 < size && (*board)[i][j] == (*board)[i][index + 1]) {
					*score = *score + (*board)[i][index + 1] * 2;
					(*board)[i][index + 1] *= 2;
					(*board)[i][j] = 0;
					(*freeTiles)++;
					index++;
				}
				else if (index != j) {
					(*board)[i][index] = (*board)[i][j];
					(*board)[i][j] = 0;

				}
				index--;
			}
		}
	}
}

void Game2048::moveDown(int*** board, int size, int* score, int* freeTiles) {
	int index;
	for (int j = size - 1; j >= 0; j--) {
		index = size - 1;
		for (int i = size - 1; i >= 0; i--) {
			if ((*board)[i][j] != 0) {
				if (index + 1 < size && (*board)[i][j] == (*board)[index + 1][j]) {
					*score = *score + (*board)[index + 1][j] * 2;
					(*board)[index + 1][j] *= 2;
					(*board)[i][j] = 0;
					(*freeTiles)++;
					index++;
				}
				else if (index != i) {
					(*board)[index][j] = (*board)[i][j];
					(*board)[i][j] = 0;
					
				}
				index--;
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
