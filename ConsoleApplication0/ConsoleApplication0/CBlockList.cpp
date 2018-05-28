
#include "stdafx.h"
#include "CBlockList.h"
#include <fstream>
#include <windows.h>
#include <iostream>
#include "picosha2.h"

const int N = 512;

using namespace std;

CBlockList::CBlockList() : size(0), hashed(0) {}

bool CBlockList::push(vector<unsigned char> &block) {
	lock_guard<mutex> lck(mtx);
	if (size < N) {
		CBlock blck;
		swap(blck.data, block);
		lst.push_front(blck);
		++size;
		cv.notify_all();
		return 1;
	}
	return 0;
}

void CBlockList::write(const char *filename) {
	ofstream fout(filename, ios::binary);
	for (CBlock &b : lst) {
		for (unsigned char &ch : b.hash) fout << ch;
	}
	fout.close();
	cout << "\n";
	system("pause");

}

bool CBlockList::hashData() {
	unique_lock<mutex> lck(mtx);
	while (size == hashed && hashed < N) {
		cv.wait(lck);
	}
	if (hashed >= N)
		return false;
	auto it = lst.begin();
	while (it->hash.size() != 0 && it != lst.end()) it++;
	it->hash.resize(picosha2::k_digest_size);
	picosha2::hash256(it->data, it->hash);
	++hashed;
	system("cls");
	cout << "loading: " << size - hashed << endl << "loaded:  " << hashed
		<< endl << hashed * 100 / N << "%" << endl;;
	return hashed < N;
}
