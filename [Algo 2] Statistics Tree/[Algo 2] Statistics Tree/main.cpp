#include "stdafx.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#define RED 1
#define BLACK 0

class student;
class group {
public:
	std::string name;
	std::vector<student*> groupList;

	group(std::string a) {
		name = a;
	}
};

class student {
public:
	std::string name;
	std::string homeCountry;
	std::string homeCity;
	double rating;
	group* specialization;

	student(std::string name, std::string country, std::string city, double rating, group* s) {
		this->name = name;
		homeCountry = country;
		homeCity = city;
		this->rating = rating;
		specialization = s;
	}

	void show() const {
		std::cout << "Student Name: " << name << "\nStudent Country: " << homeCountry << "\nStudent City: " << homeCity << "\nStudent grade: " << rating << std::endl;
	}
};

class Node {
public:
	friend class RBTree;
	Node() {
		color = RED;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
		value = nullptr;
		size = 0;
	}

	Node(student* v) {
		value = v;
		size = 1;
	}

	bool color;
	Node* left;
	Node* right;
	Node* parent;
	student* value;
	int size;
};

class RBTree {
	Node* nil;
	Node* root;
	bool mode; //0 - grades, 1 - name
	int nodes;

public:

	RBTree(bool mode)
		: nodes(0), mode(mode),nil(new Node), root(nil) {
		nil->left = 0; nil->parent = 0; nil->right = 0; nil->color = BLACK, nil->size = 0;
	}

	Node* getRoot() { return root; }
	Node* getNil() { return nil; }
	int getNodes() { return nodes; }

	void deleteFixup(Node *x);
	Node* treeSuccessor(Node* x);
	void remove(Node* element);
	void fix(Node* pivot);
	void insert(student* x);
	void rotateLeft(Node* pivot);
	void rotateRight(Node* pivot);
	Node* search(std::string key);
	void erase(std::string key);
};

void nodeCheck(Node* root, RBTree* t) {
	if (root != t->getNil()) {
		std::cout << root->value << " " << root->size << std::endl;
		nodeCheck(root->left, t);
		nodeCheck(root->right, t);
	}
}

void RBTree::rotateLeft(Node* pivot) {
	Node* y = pivot->right;
	if (pivot->parent == nil)
		root = y;
	else {
		if (pivot == pivot->parent->left)
			pivot->parent->left = y;
		else
			pivot->parent->right = y;
	}
	y->parent = pivot->parent;
	pivot->right = y->left;
	y->left->parent = pivot;
	y->left = pivot;
	pivot->parent = y;

	y->size = pivot->size;
	int l = pivot->left->size, r = pivot->right->size;
	pivot->size = l + r + 1;
}

void RBTree::rotateRight(Node* pivot) {
	Node* y = pivot->left;
	if (pivot->parent == nil)
		root = y;
	else {
		if (pivot->parent->left == pivot)
			pivot->parent->left = y;
		else
			pivot->parent->right = y;
	}
	y->parent = pivot->parent;
	pivot->left = y->right;
	y->right->parent = pivot;
	y->right = pivot;
	pivot->parent = y;

	y->size = pivot->size;
	int l = pivot->left->size, r = pivot->right->size;
	pivot->size = l + r + 1;
}

void RBTree::fix(Node* pivot) {
	while(pivot->parent->color == RED) {
		if (pivot->parent == pivot->parent->parent->left) {
			Node* y = pivot->parent->parent->right;
			if (y->color == RED) {
				pivot->parent->color = BLACK;
				y->color = BLACK;
				pivot->parent->parent->color = RED;
				pivot = pivot->parent->parent;
			}
			else {
				if (pivot == pivot->parent->right) {
					pivot = pivot->parent;
					this->rotateLeft(pivot);
				}
				pivot->parent->color = BLACK;
				pivot->parent->parent->color = RED;
				this->rotateRight(pivot->parent->parent);
			}
		}
		else {
			Node* y = pivot->parent->parent->left;
			if (y->color == RED) {
				pivot->parent->color = BLACK;
				y->color = BLACK;
				pivot->parent->parent->color = RED;
				pivot = pivot->parent->parent;
			}
			else {
				if (pivot == pivot->parent->left) {
					pivot = pivot->parent;
					rotateRight(pivot);
				}
				pivot->parent->color = BLACK;
				pivot->parent->parent->color = RED;
				rotateLeft(pivot->parent->parent);
			}
		}
	}
	root->color = BLACK;
}

void RBTree::insert(student* newS) {
	nodes++;
	Node* t = new Node(newS);
	Node* x = root;
	Node* y = nil;
	while (x != nil) {
		x->size++;
		y = x;
		if (mode) {
			if (newS->name < x->value->name) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}
		else {
			if (newS->rating < x->value->rating) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}
	}
	t->parent = y;
	if (y == nil)
		root = t;
	else {
		if (mode) {
			if (t->value->name < y->value->name)
				y->left = t;
			else
				y->right = t;
		}
		else {
			if (t->value->rating < y->value->rating)
				y->left = t;
			else
				y->right = t;
		}
	}
	t->left = nil;
	t->right = nil;
	t->color = RED;
	fix(t);
}

Node* RBTree::treeSuccessor(Node* x) {
	if (x->right != nil) {
		while (x->left != nil)
			x = x->left;
		return x;
	}
	Node* y = x->parent;
	while (y != nil && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

void RBTree::deleteFixup(Node * x) {
	while (x != root && x->color == BLACK) {
		Node* w = 0;
		if (x->parent->left == x) {
			w = x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rotateLeft(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rotateRight(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rotateLeft(x->parent);
				x = root;
			}
		}
		else {
			w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rotateRight(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					rotateLeft(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rotateRight(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

void RBTree::remove(Node* element) {
	std::cout << "HELLO\n\n" << std::endl;
	Node* temp = element;
	while (temp != nil) {
		temp->size--;
		temp = temp->parent;
	}
	Node * x = nil;
	Node * y = nil;
	if (element->left == nil || element->right == nil)
		y = element;
	else
		y = treeSuccessor(element);
	if (y->left != nil)
		x = y->left;
	else
		x = y->right;
	x->parent = y->parent;
	if (y->parent == nil)
		root = x;
	else {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}
	if (y != element) {
		if (mode) {
			element->value->name = y->value->name;
		}
		else {
			element->value->rating = y->value->rating;
		}
	}
	if (y->color == BLACK)
		deleteFixup(x);
	delete y;
	nodes--;
}

Node* RBTree::search(std::string key) {
	Node* x = root;
	while (x != nil && key != x->value->name) {
		if (key < x->value->name)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

void RBTree::erase(std::string key) {
	Node* x = search(key);
	if (x != nil) {
		remove(x);
	}
}

Node* OS_Select(Node* x, int i) {
	int r = x->left->size + 1;
	if (i == r) return x;
	else {
		if (i<r) return OS_Select(x->left, i); else
			return OS_Select(x->right, i - r);
	}
}

int OS_Rank(Node *root, Node *x) {
	int r = x->left->size + 1;
	Node *y = x;
	while (y != root) {
		if (y == y->parent->right)
			r = r + y->parent->left->size + 1;
		y = y->parent;
	}
	return r;
}

int main() {
	RBTree* gradeOriented = new RBTree(0);
	RBTree* nameOriented = new RBTree(1);
	std::vector<group*> Groups(10);
	std::vector<student*> Students(100);

	std::ifstream in("base.txt");
	std::string temp, tempCountry, tempCity, tempSpec, tempR;
	double tempRating;

	for (int i = 0; i < 10; i++) {
		getline(in, temp);
		Groups[i] = new group(temp);
	}

	getline(in, temp);

	for (int i = 0; i < 100; i++) {
		getline(in, temp);
		getline(in, tempR);
		getline(in, tempCity);
		getline(in, tempCountry);
		getline(in, tempSpec);
		tempRating = atof(tempR.c_str());
		Students[i] = new student(temp, tempCountry, tempCity, tempRating, new group(tempSpec));

		gradeOriented->insert(Students[i]);
		nameOriented->insert(Students[i]);

		for (int j = 0; j < 10; j++) {
			if (Groups[j]->name == temp) {
				Groups[j]->groupList.push_back(Students[i]);
			}
		}
		getline(in, temp);
	}

	/*int option;
	std::string searchName;

	while (1) {
		std::cout << "WHATCHA GONNA DO NOW?\n1.Delete Student\n2.Get nth student by rating\n3.Get nth student by name" << std::endl;
		std::cin >> option;
		std::cin.ignore();
		switch (option)
		{
		case 1:
			std::cout << "\nEnter his Name: " << std::endl;
			std::getline(std::cin, searchName);
			gradeOriented->erase(searchName);
			nameOriented->erase(searchName);
			std::cout << gradeOriented->getNodes() << std::endl;
			std::cout << nameOriented->getNodes() << std::endl;
			break;
		default:
			continue;
			break;
		}
	}*/
	
	system("pause");
	return 0;
}