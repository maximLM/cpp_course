#pragma once

#include <iostream>
#include <vector>
#include "picosha2.h"
using namespace std;

class Node {
	string val;
	Node *left_son, *right_son;

public:
	string get_val() {
		return val;
	}

	Node(vector<string> &blocks, int tl, int tr) {
		left_son = NULL;
		right_son = NULL;
		if (tl == tr) {
			val = picosha2::hash256_hex_string(blocks[tl]);
		}
		else {
			int tm = (tl + tr) >> 1;
			left_son = new Node(blocks, tl, tm);
			right_son = new Node(blocks, tm + 1, tr);
			val = picosha2::hash256_hex_string(left_son->get_val() + right_son->get_val());
		}
	}

	~Node() {
		if (left_son != NULL) delete left_son;
		if (right_son != NULL) delete right_son;
	}
};
