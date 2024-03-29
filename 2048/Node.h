#ifndef NODEHPP
#define NODEHPP
#include <vector>


class Node {
public:
	Node(int turn);
	Node(int move, int turn);
	Node* getParent();
	Node* getChild(int index);
	std::vector<Node*>* getChildren();
	std::vector<int> getRemainingMoves ();
	void addChild(Node* newChild);
	double getSelectionValue();
	void collectLeafNodes(std::vector<Node*>* leafNodes, int turn);
	int getSimulations();
	int getMove();
	int getTurn();
	int getScore();
	void setScore (int score);
	void setMove (int move);
	void setParent(Node* node);
	void setSimulations(int simulations);
	void setBoard (int*** newBoard, int size);
	void makeMove (int size);
	void generateRandomTile ();
	void printBoard ();
	void generateRemainingMoves();
	void rootSetupRemMoves ();
private:
	std::vector<Node*> children;
	std::vector<int> remainingMoves;
	Node* parent;
	int simulations;
	int move;
	int** board;
	int size;
	int turn;
	int score;
	int freeTiles;
};
#endif
