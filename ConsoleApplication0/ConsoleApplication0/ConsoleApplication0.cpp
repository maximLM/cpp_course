
#include "stdafx.h"
#include <iostream>
#include <thread>
#include <random>
#include "CBlockList.h"
#include "picosha2.h"

using namespace std;

void produce(CBlockList &lst) {
	while (1) {
		std::vector<unsigned char> block(512 * 1024);
		for (unsigned char& ch : block) {
			ch = rand() % 255;
		}
		if (!lst.push(block)) break;
	}
}

void consume(CBlockList &lst) {
	while (lst.hashData()) {}
}

int main() {
	CBlockList lst;
	thread p1(produce, ref(lst));
	thread p2(produce, ref(lst));
	thread c1(consume, ref(lst));
	thread c2(consume, ref(lst));
	p1.join();
	p2.join();
	c1.join();
	c2.join();
	lst.write("D:\\hashes.bin");
	return 0;
}