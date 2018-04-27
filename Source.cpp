#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

bool gameover;

struct node;
int miniMax(node* n);


struct node {
	vector<vector<char>> table;
	node* parent;
	vector<node*> child;
	unsigned int depth;
	bool maximizingPlayer;
	int value = -1;
	int move;
	bool done;

	void display() {
		for (unsigned i = 0; i < table.size(); i++) {
			for (unsigned j = 0; j < table[0].size(); j++) {
				cout << table[i][j] << ' ';
			}
			cout << endl;
		}
	}
	bool checkMove(int move) {
		move--;
		if (table[move / 3][move % 3] == '*') {
			return true;
		}
		else {
			return false;
		}
	}

	void changeTable(int move) {
		move--;
		table[move / 3][move % 3] = 'X';
		this->isDone();
		if (done) {
			gameover = true;
		}
	}
	void changeTable2(int move) {
		move--;
		table[move / 3][move % 3] = 'O';
		this->isDone();
		if (done) {
			gameover = true;
		}
	}

	void AI() {
		maximizingPlayer = true;
		vector<node*> way;
		this->child = way;
		generateChildren();
		cout << maximizingPlayer << endl;
		int k = 0;
		int biggestValue = 0;
		int temp;
		for (unsigned i = 0; i < child.size(); i++) {
			temp = miniMax(child[i]);
			if (child[i]->value == 2) {
				changeTable2(child[i]->move);
				return;
			}
			if (temp > biggestValue) {
				biggestValue = temp;
				k = i;
			}
		}
		//cout << biggestValue << endl;
		//cout << temp << endl;
		changeTable2(child[k]->move);
	}


	void generateChildren() {
		for (unsigned i = 0; i < table.size(); i++) {
			for (unsigned j = 0; j < table.size(); j++) {
				if (table[i][j] == '*') {
					child.push_back(new node);
					child.back()->parent = this;
					child.back()->table = table;
					if (maximizingPlayer) {
						child.back()->table[i][j] = 'O';
					}
					else {
						child.back()->table[i][j] = 'X';
					}
					child.back()->move = (i * 3) + (j + 1);
					child.back()->maximizingPlayer = !maximizingPlayer;
					child.back()->depth = depth + 1;
					child.back()->isDone();
					if (!child.back()->done) {
						child.back()->generateChildren();
					}
				}
			}
		}
	}

	void deleteChildren() {
		for (unsigned i = 0; i < child.size(); i++) {
			if (child[i]->child.size() != 0) {
				child[i]->deleteChildren();
			}
			else {
				delete[] this;
			}
		}
	}

	void isDone() {
		for (unsigned i = 0; i < table.size(); i++) {
			// ***
			// XXX
			// ***
			if (table[i][0] == table[i][1] && table[i][1] == table[i][2] && table[i][0] != '*') {
				if (table[i][0] == 'X') {
					value = 0;
					done = true;
					return;
				}
				else if (table[i][0] == 'O') {
					value = 2;
					done = true;
					return;
				}
			}
			// *X*
			// *X*
			// *X*
			if (table[0][i] == table[1][i] && table[1][i] == table[2][i] && table[0][i] != '*') {
				if (table[0][i] == 'X') {
					value = 0;
					done = true;
					return;
				}
				else if (table[0][i] == 'O'){
					value = 2;
					done = true;
					return;
				}
			}
		}
		// X**
		// *X*
		// **X
		if (table[0][0] == table[1][1] && table[1][1] == table[2][2] && table[0][0] != '*') {
			if (table[1][1] == 'X') {
				this->value = 0;
				done = true;
				return;
			}
			else if (table[1][1] == 'O'){
				this->value = 2;
				done = true;
				return;
			}
		}
		// **X
		// *X*
		// X**
		if (table[2][0] == table[1][1] && table[1][1] == table[0][2] && table[1][1] != '*') {
			if (table[1][1] == 'X') {
				this->value = 0;
				done = true;
				return;
			}
			else if (table[1][1] == 'O'){
				this->value = 2;
				done = true;
				return;
			}
		}
		// ***
		// ***
		// ***
		done = true;
		for (unsigned i = 0; i < table.size(); i++) {
			for (unsigned j = 0; j < table.size(); j++) {
				if (table[i][j] == '*') {
					done = false;
					return;
				}
			}
		}
		value = 1;
	}
};

int miniMax(node* n) {
	int v;
	if (!n->done) {
		if (n->maximizingPlayer) {
			int bestValue = -100;
			for (unsigned i = 0; i < n->child.size(); i++) {
				v = miniMax(n->child[i]);
				bestValue = max(bestValue, v);
			}
			return bestValue;
		}
		else {
			int bestValue = +100;
			for (unsigned i = 0; i < n->child.size(); i++) {
				v = miniMax(n->child[i]);
				bestValue = min(bestValue, v);
			}
			return bestValue;
		}
	}
	else {
		return n->value;
	}
}

int main() {
	srand(time(0));
	node init;
	init.table = { { '*', '*', '*', }, { '*', '*', '*', }, { '*', '*', '*', } };
	init.done = false;
	init.parent = NULL;
	init.maximizingPlayer = true;
	init.display();
	while (1) {
		int move;
		cout << "Enter your move: ";
		cin >> move;
		if (init.checkMove(move)) {
			init.changeTable(move);
			init.isDone();
			if (init.done) {
				if (init.value = 0) {
					system("clear");
					init.display();
					cout << "You won, congrats!     " << init.value << endl;
				}
				else if (init.value = 1) {
					system("clear");
					init.display();
					cout << "We tied!" << endl;
				}
				else {
					cout << "hhhhh" << endl;
				}
				return 0;
			}
			init.AI();
			system("clear");
			init.display();
			if (gameover) {
				system("clear");
				init.display();
				cout << "I won, maybe next time!" << endl;
				return 0;
			}
			init.maximizingPlayer = true;
		}
		else {
			cout << "Tile is full." << endl;
		}
		init.maximizingPlayer = false;
	}
	return 0;
}
