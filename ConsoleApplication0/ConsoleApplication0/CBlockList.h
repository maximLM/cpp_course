
#include "stdafx.h"
#include <mutex>
#include <atomic>
#include <forward_list>
#include <condition_variable>
#include "cblock.h"

using namespace std;

class CBlockList {
	forward_list<CBlock> lst;
	mutex mtx;
	condition_variable cv;
	atomic<size_t> size;
	atomic<size_t> hashed;

public:
	CBlockList();
	bool push(vector<unsigned char> &block);
	bool hashData();
	void write(const char* filename);
};
