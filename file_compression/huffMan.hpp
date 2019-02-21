#include "fileCompressHuff.hpp"

template <class T>
struct huffManNode {
	huffManNode(const T& weight = T()) : _weight(weight), _left(NULL), _right(NULL), _parent(NULL) {}

	struct huffManNode<T> *_left;
	struct huffManNode<T> *_right;
	struct huffManNode<T> *_parent;
	T _weight;
};


struct Compare {
	bool operator()(const huffManNode<CharInfo>* w1, const huffManNode<CharInfo>* w2) {
		return w1->_weight._charCount > w2->_weight._charCount;
	}
};

template <class T>
class huffManTree {
	typedef struct huffManNode<T> HNode;
	private:
		HNode *_HRoot;
	public:
		huffManTree() : _HRoot(NULL) {}

		void createHuffmanTree(std::vector<T>& tmpWeight, T& invalid) {
			if(tmpWeight.size() == 0) {
				return;
			}
			std::priority_queue<HNode*, std::vector<HNode*>, Compare> creatHeap;
			
			for(size_t i = 0; i < tmpWeight.size(); i++) {
				if(tmpWeight[i] != invalid) {
					HNode *pCur = new HNode(tmpWeight[i]);
					creatHeap.push(pCur);
				}
			}

			while(!creatHeap.empty() && creatHeap.size() != 1) {
				HNode *left = creatHeap.top();
				creatHeap.pop();

				HNode *right = creatHeap.top();
				creatHeap.pop();

				HNode *newNode = new HNode(left->_weight + right->_weight);
				newNode->_left = left;
				newNode->_right = right;
				left->_parent = newNode;
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

		void getCode(HNode *hRoot) {
			if(hRoot == NULL) {
				return;
			}

			getCode(hRoot->_left);
			getCode(hRoot->_right);

			if(hRoot->_left == NULL && hRoot->_right == NULL) {
				HNode *parent = hRoot->_parent;
				HNode *cur = hRoot;
				while(parent) {
					if(parent->_left == cur) {
						hRoot->_weight._charCode.insert(0, "0");
					}

					if(parent->_right == cur) {
						hRoot->_weight._charCode.insert(0, "1");
					}

					cur = parent;
					parent = cur->_parent;
				}
			}
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
