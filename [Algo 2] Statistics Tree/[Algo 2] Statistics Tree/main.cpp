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
		std::cout << "Student Name: " << name << "\nStudent Country: " << homeCountry << "\nStudent City: " << homeCity << std::endl;
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
		value = 0;
	}

	Node(int v) {
		value = v;
	}

	bool color;
	Node* left;
	Node* right;
	Node* parent;
	int value;
	int size;
};

class RBTree {
	Node* nil;
	Node* root;

public:

	RBTree()
		:nil(new Node), root(nil) {
		nil->left = 0; nil->parent = 0; nil->right = 0; nil->color = BLACK;
	}

	void deleteFixup(Node *x);
	Node* treeSuccessor(Node* x);
	void remove(Node* element);
	void fix(Node* pivot);
	void insert(int value);
	void rotateLeft(Node* pivot);
	void rotateRight(Node* pivot);
	Node* search(int key);
	void erase(int key);
	void updateSizes(Node* current);
};

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

void RBTree::insert(int key) {
	Node* t = new Node(key);
	Node* x = root;
	Node* y = nil;
	while (x != nil) {
		y = x;
		if (key < x->value)
			x = x->left;
		else
			x = x->right;
	}
	t->parent = y;
	if (y == nil)
		root = t;
	else {
		if (t->value < y->value)
			y->left = t;
		else
			y->right = t;
	}
	t->left = nil;
	t->right = nil;
	t->color = RED;
	fix(t);
	updateSizes(root);
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
	if (y != element)
		element->value = y->value;
	if (y->color == BLACK)
		deleteFixup(x);
	delete y;
	updateSizes(root);
}

Node* RBTree::search(int key) {
	Node* x = root;
	while (x != nil && key != x->value) {
		if (key < x->value)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

void RBTree::erase(int key) {
	Node* x = search(key);
	if (x != nil) {
		remove(x);
	}
}

void updateSizes(Node* current) {
	if (!current->left && !current->right) {
		current->size = 1;
		return;
	}
	
	int s = 1;
	if(current->left) {
		updateSizes(current->left);
		s += current->left->size;
	}
	if (current->right) {
		updateSizes(current->right);
		s += current->right->size;
	}

	current->size = s;
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
	RBTree gradeOriented;
	RBTree nameOriented;
	std::vector<group*> Groups(10);
	std::vector<student*> Students(100);

	std::ifstream in("base.txt");
	std::string temp, tempCountry, tempCity, tempSpec, tempR;
	double tempRating;

	system("pause");
	return 0;
}