#pragma once
#include "Node.h"

class AI {
public:
	AI(int size);
	void generateMove(int*** board, int freeTiles, int* output, int threads);
private:
	void constructTree(Node* root);
	int size;
	Node* nodeSelect(Node* root, int player);
	void nodeExpand(Node* node);
	void nodeBackpropagate(Node* node, int outcome, int player);
	void generateOutcome(Node* root, int iterations);
};
