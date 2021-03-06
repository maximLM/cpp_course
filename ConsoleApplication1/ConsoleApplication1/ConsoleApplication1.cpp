#include "stdafx.h"
#include <iostream>
#include "picosha2.h"
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <queue>
#include <fstream>
#include "merkle_tree.h"


using namespace std;

// picosha2::hash256_hex_string(src)

mutex mtx;
queue<string> que;
condition_variable cv;
const int TOTAL_HASHS = 128;
const int SIZE_OF_HASH = 4;
const int LENGTH_OF_STRING = 512;

string generate() {
	string ret = "";
	for (int i = 0; i < LENGTH_OF_STRING; ++i) {
		ret += char('a' + rand() % 26);
	}
	return ret;
}


int producer() {
	int ret = 0;
	int cnt = TOTAL_HASHS * SIZE_OF_HASH;
	while (cnt--) {
		string str = generate();
		mtx.lock();
		que.push(str);
		if (que.size() >= SIZE_OF_HASH) cv.notify_all();
		mtx.unlock();
	}
	return ret;
}

bool ready() {
	mtx.lock();
	bool ret = que.size() >= SIZE_OF_HASH;
	mtx.unlock();
	return ret;
}


void consumer() {
	int cnt = TOTAL_HASHS;
	ofstream fout("log.txt");
	while (cnt--) {
		while (!ready()) {
			unique_lock<mutex> lck(mtx);
			cv.wait(lck);
		}
		mtx.lock();
		vector<string> blocks;
		for (int i = 0; i < SIZE_OF_HASH; ++i) {
			blocks.push_back(que.front());
			que.pop();
		}
		mtx.unlock();
		Node * node = new Node(blocks, 0, SIZE_OF_HASH - 1);
		fout << node->get_val() << endl;
		delete node;
	}
}

int main()
{
	thread consumer_thread(consumer);
	thread producer_thread(producer);
	consumer_thread.join();
	producer_thread.join();
    return 0;
}

