#ifndef GAMEHPP
#define GAMEHPP

class Game2048 {
public:
	Game2048(int size);
	static void generateRandomTile(int*** board, int size, int* score, int* freeTiles);
	static void executeMove(int*** board, int size, int move, int* score, int* freeTiles, int turn);
	static void setupBoard(int size, int*** board, int* score, int* freeTiles);
	static void printBoard(int*** board, int size);

	static int searchDepth;

private:
	int gameSize;
	int** board;
	bool lkey;
	bool rkey;
	bool ukey;
	bool dkey;
	bool gameLost;
	void getMove(int* move);
	
	static void moveLeft(int*** board, int size, int* score, int* freeTiles);
	static void rotateCW(int*** board, int size);
	static void rotateCCW(int*** board, int size);
};

#endif
