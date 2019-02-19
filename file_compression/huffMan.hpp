#include "fileCompressHuff.hpp"

template <class T>
struct huffManNode {
	huffManNode(const T& weight = T()) : _weight(weight), _left(NULL), _right(NULL), _parent(NULL) {}

	struct huffManNode<T> *_left;
	struct huffManNode<T> *_right;
	struct huffManNode<T> *_parent;
	T _weight;
};


