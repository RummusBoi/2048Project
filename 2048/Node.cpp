#include "Node.h"
#include "2048.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

#define BIAS 1.4

Node::Node (int turn){
	simulations = 0;
	baseScore = 0;
	accScore = 0;
	parent = NULL;
	this->turn = turn;
	remainingMoves = std::vector<int>();
}

Node::Node (int move, int turn) {
	simulations = 0;
	baseScore = 0;
	accScore = 0;
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
	(*newChild).setScore(baseScore);
	
	newChild->setBoard(&board, size);
	
	newChild->setParent(this, nodeDepth);
	
	newChild->setFreeTiles (freeTiles);

	newChild->makeMove(size);
	newChild->setAccScore(newChild->baseScore);
	
	//if turn of child is 1, finds the tiles where a random tile can be placed by game
	//TODO set tilesLeft and set remainingmoves for turn%2=0 and turn%2=1
	newChild->generateRemainingMoves();
	
}

void Node::rootSetupRemMoves() { 
	int** cmpBoard = new int* [size];
	for (int i = 0; i < size; i ++) {
		cmpBoard[i] = new int[size];
	}
	
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cmpBoard[i][j] = board[i][j];
		}
	}

	int freeTiles = 16;
	int score = 0;
	
	for (int move = 1; move <= 4; move ++) {
		Game2048::executeMove(&cmpBoard, size, move, &score, &freeTiles, 1);
		if (!cmpBoards(&cmpBoard, &board, size)) {
			remainingMoves.push_back(move);
		}

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				cmpBoard[i][j] = board[i][j];
			}
		}
		
		
	}

	nodeDepth = 0;
}

bool Node::cmpBoards(int*** board1, int ***board2, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((*board1)[i][j] != (*board2)[i][j]){
				return false;
			}
		}
	}
	return true;
}

void Node::generateRemainingMoves() {
	Game2048::generateRemainingMovesFromBoard((&board), size, turn, &freeTiles, (&remainingMoves));
}

//new implementation of nodesimulate may not work. Changed the location of nodeSimulate from AI.cpp to Node.cpp. 

int Node::nodeSimulate() {
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
		if (tmpTurn >= 200) done = true;
		
	}
	return tmpScore + baseScore;
}

int Node::getNodeDepth() {
	return nodeDepth;
}

Node* Node::getChild(int index) {
	return (children[index]);
}

std::vector<Node*>* Node::getChildren() {
	return &children;
}

std::vector<int>* Node::getRemainingMoves(){
	return &remainingMoves;
}


void Node::setScore(int newScore) {
	baseScore = newScore;
}

int Node::getMove() {
	return move;
}

int Node::getTurn() {
	return turn;
}

int Node::getScore() {
	return baseScore;
}

int Node::getAccScore() {
	return accScore;
}

void Node::setAccScore(int newScore) {
	accScore = newScore;
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

void Node::setFreeTiles(int freeTiles) {
	this->freeTiles = freeTiles;
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
	Game2048::executeMove(&board, size, move, &baseScore, &freeTiles, turn);
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
	return (double)accScore / (double)simulations + BIAS * sqrt(log((double)parent->getSimulations()) / (double)simulations);
}
