#include "Node.h"
#include <math.h>
#include "2048.h"
#include <iostream>

#define BIAS 1

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

void Node::addChild(Node* newChild) {
	children.push_back(newChild);
	int index = rand()% remainingMoves.size();
	(*newChild).setMove(remainingMoves[index]);
	remainingMoves.erase(remainingMoves.begin() + index);
	freeTiles--;
	
	std::cout << "hello guys" << std::endl
	
	newChild->setBoard(&board, size);
	
	newChild->setParent(this);
	
	//if turn of child is 1, finds the tiles where a random tile can be placed by game
	//TODO set tilesLeft and set remainingmoves for turn=0 and turn=1
	newChild->generateRemainingMoves();
	
}

void Node::rootSetupRemMoves() {
	for(int i = 1; i <= 4; i++) {
		remainingMoves.push_back(i);
	}
	
}

void Node::generateRemainingMoves() {
	//for now, adds all four moves if turn%2==0
	if (turn%2==0){
		remainingMoves.push_back(0);
		remainingMoves.push_back(1);
		remainingMoves.push_back(2);
		remainingMoves.push_back(3);
	}
	freeTiles = size*size;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] != 0) {
				freeTiles--;
				if (turn%2 == 1) {
					std::cout << i + " : " + j + " is not 0" << std::endl;
					remainingMoves.push_back(i + 4 * j);
					remainingMoves.push_back(i + 4 * j + 16);
				}
			}
		}
	}

	
	
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

void Node::setParent(Node* node) {
	parent = node;
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
	Game2048::executeMove(&board, size, move, &score, &freeTiles);
}

void Node::collectLeafNodes(std::vector<Node*>* leafNodes, int turn) {
	if (children.size() < (turn == 0 ? 4 : 32) && this->turn == turn) (*leafNodes).push_back(this);
	
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
