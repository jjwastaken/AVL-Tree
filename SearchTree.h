#pragma once
template <typename E>
class SearchTree {					// a binary search tree
public: 						// public types
	typedef typename E::Key K;				// a key
	typedef typename E::Value1 V1;			// a value1
	class Iterator;					// an iterator/position
public:						// public functions
	SearchTree();					// constructor
	int size() const; 					// number of entries
	bool empty() const;					// is the tree empty?
	Iterator find(const K& k);				// find entry with key k
	Iterator insert(const K& k, const V1& x);		// insert (k,x)
	void erase(const K& k) throw(NonexistentElement);	// remove key k entry
	void erase(const Iterator& p);			// remove entry at p
	Iterator begin();					// iterator to first entry
	Iterator end();					// iterator to end entry
protected:						// local utilities
	typedef BinaryTree<E> BinaryTree;			// linked binary tree
	typedef typename BinaryTree::Position TPos;		// position in the tree
	TPos root() const;					// get virtual root
	TPos finder(const K& k, const TPos& v);		// find utility
	TPos inserter(const K& k, const V1& x);		// insert utility
	TPos eraser(TPos& v);				// erase utility
	TPos restructure(const TPos& v) 			// restructure
		throw(BoundaryViolation);
private: 						// member data
	BinaryTree T;					// the binary tree
	int n;						// number of entries
public:
	// ...insert Iterator class declaration here
	class Iterator {	                      		// an iterator/position
	private:
		TPos v;						// which entry
	public:
		Iterator(const TPos& vv) : v(vv) { }		// constructor
		const E& operator*() const { return *v; }	// get entry (read only)
		E& operator*() { return *v; }			// get entry (read/write)
		bool operator==(const Iterator& p) const		// are iterators equal?
		{
			return v == p.v;
		}
		Iterator& operator++();				// inorder successor
		friend class SearchTree;				// give search tree access
	};
};

template <typename E>					// constructor
SearchTree<E>::SearchTree() : T(), n(0)
{
	T.addRoot(); T.expandExternal(T.root());
}	// create the super root

template <typename E>					// get virtual root
typename SearchTree<E>::TPos SearchTree<E>::root() const
{
	return T.root().left();
}				// left child of super root

template <typename E>					// find utility
typename SearchTree<E>::TPos SearchTree<E>::finder(const K& k, const TPos& v) {
	if (v.isExternal()) return v;			// key not found
	if (k < v->key()) return finder(k, v.left());	// search left subtree
	else if (v->key() < k) return finder(k, v.right());	// search right subtree
	else return v;					// found it here
}

template <typename E>					// find entry with key k
typename SearchTree<E>::Iterator SearchTree<E>::find(const K& k) {
	TPos v = finder(k, root());				// search from virtual root
	if (v.isInternal()) return Iterator(v);		// found it
	else return end();					// didn't find it
}

template <typename E>					// insert utility
typename SearchTree<E>::TPos SearchTree<E>::inserter(const K& k, const V1& x) {
	TPos v = finder(k, root());				// search from virtual root
	while (v.isInternal())				// key already exists?
		v = finder(k, v.right());				// look further
	T.expandExternal(v);				// add new internal node
	v->setKey(k); v->setValue1(x);			// set entry
	n++;						// one more entry
	return v;						// return insert position
}

template <typename E>					// insert (k,x)
typename SearchTree<E>::Iterator SearchTree<E>::insert(const K& k, const V1& x)
{
	TPos v = inserter(k, x); return Iterator(v);
}

template <typename E>					// remove utility
typename SearchTree<E>::TPos SearchTree<E>::eraser(TPos& v) {
	TPos w;
	if (v.left().isExternal()) w = v.left();		// remove from left
	else if (v.right().isExternal()) w = v.right();	// remove from right
	else {						// both internal?
		w = v.right();					// go to right subtree
		do { w = w.left(); } while (w.isInternal());	// get leftmost node
		TPos u = w.parent();
		v->setKey(u->key()); v->setValue1(u->value1());	// copy w's parent to v
	}
	n--;						// one less entry
	return T.removeAboveExternal(w);			// remove w and parent
}

template <typename E>					// remove key k entry
void SearchTree<E>::erase(const K& k) throw(NonexistentElement) {
	TPos v = finder(k, root());				// search from virtual root
	if (v.isExternal())					// not found?
		throw NonexistentElement("Erase of nonexistent");
	eraser(v);						// remove it
}

template <typename E>					// erase entry at p
void SearchTree<E>::erase(const Iterator& p)
{
	eraser(p.v);
}

template <typename E>					// inorder successor
typename SearchTree<E>::Iterator& SearchTree<E>::Iterator::operator++() {
	TPos w = v.right();
	if (w.isInternal()) {				// have right subtree?
		do { v = w; w = w.left(); }			// move down left chain
		while (w.isInternal());
	}
	else {
		w = v.parent();					// get parent
		while (v == w.right())				// move up right chain
		{
			v = w; w = w.parent();
		}
		v = w;						// and first link to left
	}
	return *this;
}

template <typename E>					// iterator to first entry
typename SearchTree<E>::Iterator SearchTree<E>::begin() {
	TPos v = root();					// start at virtual root
	while (v.isInternal()) v = v.left();		// find leftmost node
	return Iterator(v.parent());
}

template <typename E>					// iterator to end entry
typename SearchTree<E>::Iterator SearchTree<E>::end()
{
	return Iterator(T.root());
}

