#include "AI.h"
#include "2048.h"
#include <iostream>

using namespace std;

AI::AI(int size) {
	this->size = size;
}

int AI::generateMove(int*** board, int freeTiles) {
	Node root(0);
	root.setBoard(board, size);
	root.rootSetupRemMoves();
	
	//Create 4 children for the root:
	int remSize = (int)root.getRemainingMoves()->size();
	for (int i = 0; i < remSize; i++) {
		Node* newNode = new Node (1);
		root.addChild(newNode);
		
		cout << newNode->getScore() << endl;
		nodeBackpropagate(newNode, newNode->nodeSimulate(), 0);
	}
	cout << root.getScore() << " : " << root.getAccScore() << endl;
	//first turn is given to computer, as the ai already has created 4 nodes.
	for (int step = 1; step < 100; step++) {
		int player = step % 2;
		for (int i = 0; i < (player == 1 ? 4 : 1); i++) {
			Node* newNode = new Node((int)((player + 1) % 2));

			Node* selectedNode = nodeSelect(&root, player);

			if (selectedNode == NULL) return 0;

			// ---
			//expand selected node:

			(*selectedNode).addChild(newNode);
			// ---
			//simulate game from newNode:
			int outcome = newNode->nodeSimulate();
			//backpropagate outcome through tree
			nodeBackpropagate(newNode, outcome, player);
		}
		
	}
	
	Node hiVal = *(root.getChild(0));
	double hiScore = (double)hiVal.getAccScore() / (double)hiVal.getSimulations();

	cout << "Confidence: " << endl << endl;
	
	int totalScore = 0;

	for (Node* node : *(root.getChildren())) {
		totalScore += double((*node).getAccScore()) / double((*node).getSimulations());
		
		if (double((*node).getAccScore()) / double((*node).getSimulations()) > hiScore) {
			hiVal = (*node);
			hiScore = double((*node).getAccScore()) / double((*node).getSimulations());
		}
	}

	

	for (Node* node : *(root.getChildren())) {
		char move = Game2048::charFromMove(node->getMove());
		cout << "NODE " << move << ": " << endl << "AccScore: " << node->getAccScore() << endl << "BaseScore: " << node->getScore() << endl <<
			"Simulations: " << node->getSimulations() << endl << "Node depth: " << node->getNodeDepth() << endl;
		cout << Game2048::charFromMove(node->getMove()) << " : " << 100. * double((*node).getAccScore()) / double((*node).getSimulations()) / double(totalScore) << endl;
		cout << endl;
	}

	cout << "-- selected " << Game2048::charFromMove(hiVal.getMove()) << endl;

	cout << "--------" << endl << endl;

	cout << endl;
	Game2048::printBoard(board, size);

	return hiVal.getMove();
}

Node* AI::nodeSelect(Node* root, int player) {
	std::vector<Node*> leafNodes;
	(*root).collectLeafNodes(&leafNodes, player);
	
	if (leafNodes.size() == 0) {
		return NULL;
	}
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
	
	//if player is computer, pick the node with the lowest selection value -- NOTE: changed to random node instead.
	if (player == 1) {
		/*
		for (int i = 0; i < leafNodes.size(); i++) {
			if ((*leafNodes[i]).getSelectionValue() < hiValD) {
				hiValD = (*leafNodes[i]).getSelectionValue();
				hiVal = i;
			}
		}
		*/
		hiVal = rand() % (leafNodes).size();
		//cout << "selected " << hiVal << endl;
	}
	
	return leafNodes[hiVal];
	
}

void AI::nodeExpand(Node* node) {
	Node newNode (rand() % 4);
	(*node).addChild(&newNode);
	newNode.nodeSimulate();
}

void AI::nodeBackpropagate(Node* node, int outcome, int player) {
	node->setAccScore (node->getAccScore() + outcome);
	node->setSimulations(node->getSimulations() + 1);
	if (node->getParent() != NULL) nodeBackpropagate(node->getParent(), outcome, player);
}