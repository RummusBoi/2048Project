#include "AI.h"
#include "2048.h"
#include <iostream>

using namespace std;

AI::AI(int size) {
	this->size = size;
}

int AI::generateMove(int*** board) {
	Node root(0);
	root.setBoard(board, size);
	root.rootSetupRemMoves();
	cout << "printing root board.." << endl;
	root.printBoard();
	cout << "printing original board.. " << endl;
	Game2048::printBoard(board, size);
	//Create 4 children for the root:
	for (int i = 0; i < 4; i++) {
		Node* newNode = new Node (1);
		root.addChild(newNode);
		newNode->setParent(&root);
		
		newNode->makeMove(size);
		cout << "child " << i << endl << endl;
		newNode->printBoard();
		
		
		nodeBackpropagate(newNode, nodeSimulate(newNode), 0);
	}
	
	
	
	cout << "finished setup" << endl;
	
	cout << "creating tree" << endl;
	//first turn is given to computer, as the ai already has created 4 nodes.
	for (int step = 1; step < 200; step++) {
		int player = step % 2;
		Node* newNode = new Node ((int)((player+1)%2));
		
		Node* selectedNode = nodeSelect(&root, player);
		// ---
		//expand selected node:
		
		(*selectedNode).addChild(newNode);
		// ---
		//simulate game from newNode:
		int outcome = nodeSimulate(newNode);
		//backpropagate outcome through tree
		if (player == 1) outcome = 0;
		nodeBackpropagate(newNode, outcome, player);
	}
	
	Node hiVal = *(root.getChild(0));
	int hiSim = hiVal.getSimulations();

	cout << "Finding move with highest amount of simulations.. " << endl;
	
	for (Node* node : *(root.getChildren())) {
		if ((*node).getSimulations() > hiSim) {
			hiVal = (*node);
			hiSim = hiVal.getSimulations();
		}
	}
	
	cout << "Returning move.. " << endl;

	return hiVal.getMove();
}

Node* AI::nodeSelect(Node* root, int player) {
	std::vector<Node*> leafNodes;
	(*root).collectLeafNodes(&leafNodes, player);

	double hiValD = (*leafNodes[0]).getSelectionValue();
	int hiVal = 0;

	//if player is AI, pick the node with highest selection value
	if (player == 0) {
		for (int i = 0; i < leafNodes.size(); i++) {
			if ((*leafNodes[i]).getSelectionValue() > hiValD) {
				hiValD = (*leafNodes[i]).getSelectionValue();
				hiVal = i;
			}
		}
	}
	
	//if player is computer, pick the node with the lowest selection value
	if (player == 1) {
		for (int i = 0; i < leafNodes.size(); i++) {
			if ((*leafNodes[i]).getSelectionValue() < hiValD) {
				hiValD = (*leafNodes[i]).getSelectionValue();
				hiVal = i;
			}
		}
	}
	return leafNodes[hiVal];
}

void AI::nodeExpand(Node* node) {
	Node newNode (rand() % 4);
	(*node).addChild(&newNode);
	nodeSimulate(&newNode);
}

int AI::nodeSimulate(Node* node) {
	int outcome = rand() % 100;
	return outcome;
}

void AI::nodeBackpropagate(Node* node, int outcome, int player) {
	node->setScore (node->getScore() + outcome);
	node->setSimulations(node->getSimulations() + 1);
	if (node->getParent() != NULL) nodeBackpropagate(node->getParent(), outcome, player);
}
