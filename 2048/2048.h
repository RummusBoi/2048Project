#ifndef GAMEHPP
#define GAMEHPP
#include <vector>

class Game2048 {
public:
	Game2048(int size);
	static void generateRandomTile(int*** board, int size, int* score, int* freeTiles);
	static void executeMove(int*** board, int size, int move, int* score, int* freeTiles, int turn);
	static void setupBoard(int size, int*** board, int* score, int* freeTiles);
	static void printBoard(int*** board, int size);
	static void generateRemainingMovesFromBoard(int*** board, int size, int turn, int* freeTiles, std::vector<int>* remainingMoves);

	static int searchDepth;
	static char charFromMove(int move);


	static void getMove(int* move);

private:
	int gameSize;
	int** board;
	static bool lkey;
	static bool rkey;
	static bool ukey;
	static bool dkey;
	bool gameLost;
	
	static void moveLeft(int*** board, int size, int* score, int* freeTiles);
	static void moveRight(int*** board, int size, int* score, int* freeTiles);
	static void moveDown(int*** board, int size, int* score, int* freeTiles);
	static void moveUp(int*** board, int size, int* score, int* freeTiles);
	static void rotateCW(int*** board, int size);
	static void rotateCCW(int*** board, int size);
};

#endif
