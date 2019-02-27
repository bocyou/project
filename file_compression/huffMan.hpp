#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <queue>
#include <ostream>
#include <algorithm>

typedef unsigned char UCH;

struct CharInfo {

	CharInfo(size_t count = 0) :  _charCount(count) {}

	UCH _ch;
	size_t _charCount;
	std::string _charCode;

	bool operator!=(const CharInfo& ch) {
		return _charCount != ch._charCount;
	}

	bool operator==(const CharInfo& ch) {
		return _charCount == ch._charCount;
	}

	CharInfo operator+(const CharInfo& ch) {
		return CharInfo(_charCount + ch._charCount);
	}

	friend std::ostream& operator<<(std::ostream& out, const CharInfo& ch) {
		out << ch._ch << " " << ch._charCode <<  " " << ch._charCount << "\n";
		return out;
	}
};

//template <class T>
struct huffManNode {
	huffManNode(const CharInfo& weight) : _weight(weight), _left(NULL), _right(NULL), _parent(NULL) {}

	CharInfo _weight;
	struct huffManNode *_left;
	struct huffManNode *_right;
	struct huffManNode *_parent;
};


struct Compare {
	bool operator()(const huffManNode* w1, const huffManNode* w2) {
		return w1->_weight._charCount > w2->_weight._charCount;
	}
};

//template <class T>
class huffManTree {
	typedef struct huffManNode HNode;
	private:
		HNode *_HRoot;
	public:
		huffManTree() : _HRoot(NULL) {}

		void createHuffmanTree(std::vector<CharInfo>& tmpWeight, CharInfo& invalid) {
			if(tmpWeight.size() == 0) {
				return;
			}
			std::priority_queue<HNode*, std::vector<HNode*>, Compare> creatHeap;
			
			for(size_t i = 0; i < tmpWeight.size(); i++) {
				if(tmpWeight[i] != invalid) {
					creatHeap.push(new HNode(tmpWeight[i]));
				}
			}

			while(!creatHeap.empty() && creatHeap.size() != 1) {
				HNode *left = creatHeap.top();
				creatHeap.pop();

				HNode *right = creatHeap.top();
				creatHeap.pop();

				HNode *newNode = new HNode(left->_weight + right->_weight);
				newNode->_left = left;
				left->_parent = newNode;

				newNode->_right = right;
				right->_parent = newNode;

				creatHeap.push(newNode);
			}

			_HRoot = creatHeap.top();
		}

		void displayCode(HNode *hRoot) {
			if(hRoot == NULL) {
				return;
			}

			std::cout << hRoot->_weight << std::endl;
			displayCode(hRoot->_left);
			displayCode(hRoot->_right);
		}


		HNode *getRoot() {
			return _HRoot;
		}

		~huffManTree() {
			destroy(_HRoot);
		}
	private:
		void destroy(HNode *pRoot) {
			if(pRoot == NULL) {
				return;
			}

			destroy(pRoot->_left);
			destroy(pRoot->_right);
			delete pRoot;
			pRoot = NULL;
		}
};
