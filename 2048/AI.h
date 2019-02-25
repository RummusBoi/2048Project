#pragma once
#include "Node.h"

class AI {
public:
	AI(int size);
	int generateMove(int*** board, int freeTiles);
private:
	void constructTree(Node* root);
	int size;
	Node* nodeSelect(Node* root, int player);
	int nodeSimulate(Node* node);
	void nodeExpand(Node* node);
	void nodeBackpropagate(Node* node, int outcome, int player);
};
