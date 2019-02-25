#include "Node.h"
#include "2048.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

#define BIAS 1.4

Node::Node (int turn){
	simulations = 0;
	score = 0;
	parent = NULL;
	this->turn = turn;
	remainingMoves = std::vector<int>();
}

Node::Node (int move, int turn) {
	simulations = 0;
	score = 0;
	(*this).move = move;
	
	parent = NULL;
	this->turn = turn;
	remainingMoves = std::vector<int>();
}

using namespace std;

void Node::addChild(Node* newChild) {
	children.push_back(newChild);
	int index = rand()% remainingMoves.size();
	
	(*newChild).setMove(remainingMoves[index]);
	remainingMoves.erase(remainingMoves.begin() + index);
	freeTiles--;
	
	newChild->setBoard(&board, size);
	
	newChild->setParent(this, nodeDepth);

	newChild->makeMove(size);
	
	//if turn of child is 1, finds the tiles where a random tile can be placed by game
	//TODO set tilesLeft and set remainingmoves for turn%2=0 and turn%2=1
	newChild->generateRemainingMoves();
	
}

void Node::rootSetupRemMoves() { 
	for(int i = 1; i <= 4; i++) {
		remainingMoves.push_back(i);
	}
	nodeDepth = 0;
}

void Node::generateRemainingMoves() {
	//for now, adds all four moves if turn%2==0
	/*if (turn%2==0){
		remainingMoves.push_back(1);
		remainingMoves.push_back(2);
		remainingMoves.push_back(3);
		remainingMoves.push_back(4);
	}
	freeTiles = size*size;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				freeTiles--;
				if (turn%2 == 1) {
					remainingMoves.push_back(i + 4 * j);
					remainingMoves.push_back(i + 4 * j + 16);
				}
			}
		}
	}*/

	//testing call to Game2048:
	Game2048::generateRemainingMovesFromBoard((&board), size, turn, &freeTiles, (&remainingMoves));

}

//new implementation of nodesimulate may not work. Changed the location of nodeSimulate from AI.cpp to Node.cpp. 

int Node::nodeSimulate() {
	int outcome = rand() % 100;
	int** simBoard = new int*[size];
	for (int i = 0; i < size; i++) {
		simBoard[i] = new int[size];
		for (int j = 0; j < size; j++) {
			simBoard[i][j] = board[i][j];
		}
	}

	int tmpTurn = (turn+1)%2;
	int tmpScore = 0;
	int tmpFreeTiles = freeTiles;

	bool done = false;

	//turn 1 is ai
	//turn 0 is computer

	vector<int> tmpRemainingMoves = vector<int>();
	
	while (!done) {

		/*cout << tmpFreeTiles << endl;
		cout << "score: " << tmpScore << endl;
		cout << "current board:" << endl;
		Game2048::printBoard(&simBoard, size);
		*/
		if (tmpTurn % 2 == 1 && tmpFreeTiles == 0) {done = true; break;}
		Game2048::generateRemainingMovesFromBoard(&simBoard, size, tmpTurn, &tmpFreeTiles, &tmpRemainingMoves);
		int simMove = tmpRemainingMoves[rand() % tmpRemainingMoves.size()];
		Game2048::executeMove(&simBoard, size, simMove, &tmpScore, &tmpFreeTiles, (tmpTurn+1)%2);

		tmpTurn++;
	}

	return tmpScore;
}

Node* Node::getChild(int index) {
	return (children[index]);
}

std::vector<Node*>* Node::getChildren() {
	return &children;
}

void Node::setScore(int newScore) {
	score = newScore;
}

int Node::getMove() {
	return move;
}

int Node::getTurn() {
	return turn;
}

int Node::getScore() {
	return score;
}

void Node::setMove (int move) {
	(*this).move = move;
}

void Node::setParent(Node* node, int parentNodeDepth) {
	parent = node;
	nodeDepth = parentNodeDepth + 1;
	if (nodeDepth > Game2048::searchDepth) Game2048::searchDepth = nodeDepth;
}

Node* Node::getParent() {
	return parent;
}

void Node::setSimulations(int simulations) {
	this->simulations = simulations;
}

void Node::setBoard(int*** newBoard, int size) {
	
	board = new int*[size];
	/*
	for (int i = 0; i < size; i++) {
		board[i] = new int[size];
		memcpy((board[i]), ((*newBoard)[i]), size);
	}
	*/
	for (int i = 0; i < size; i ++) {
		board[i] = new int[size];
		for (int j = 0; j < size; j++) {
			board[i][j] = (*newBoard)[i][j];
		}
	}
	this->size = size;
	
}

void Node::printBoard(){
	if (size > 50) {
		std::cout << "size is greater than 50, could not print board." << std::endl;
		return;
	}
	Game2048::printBoard(&board, size);
}

void Node::makeMove(int size) {
	Game2048::executeMove(&board, size, move, &score, &freeTiles, turn);
}

void Node::collectLeafNodes(std::vector<Node*>* leafNodes, int turn) {
	if (remainingMoves.size() > 0 && this->turn == turn) (*leafNodes).push_back(&*this);
	
	for (Node* node : children) {
		(*node).collectLeafNodes(leafNodes, turn);
	}
}

int Node::getSimulations() {
	return simulations;
}

double Node::getSelectionValue() {
	if (simulations == 0) return 0;
	return (double)score / (double)simulations + BIAS * sqrt(log(parent->getSimulations()) / simulations);
}