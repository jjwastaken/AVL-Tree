#ifndef BST_H
#define BST_H
// Modified for CSCI 220 Fall 15
// Updated for CSCI 220 Fall 19 -- overload more operators and no duplicates

#include "BinaryTree.h"
#include "RuntimeException.h"

template <typename E>
class SearchTree {				// a binary search tree
public: 							// public types
	typedef typename E::Key K;		// a key
	typedef typename E::Value1 V1;	// a value1
	typedef typename E::Value2 V2;
	class Iterator;					// an iterator/position
public:							// public functions
	SearchTree() : T(), n(0)
	{
		T.addRoot(); T.expandExternal(T.root());
	}// create the super root

	int size() const { return n; } 				// number of entries
	bool empty() const { return n == 0; };		// is the tree empty?
	Iterator find(const K& k) {
		TPos v = finder(k, root());				// search from virtual root
		if (!v.isExternal()) return Iterator(v);// found it
		else return end();						// didn't find it
	}

	Iterator insert(const K& k, const V1& x1, const V2& x2)		// insert (k,x)
	{
		TPos v = inserter(k, x1, x2); return Iterator(v);
	}

	void erase(const K& k) { //throw(NonexistentElement) {
		TPos v = finder(k, root());				// search from virtual root
		if (v.isExternal())					// not found?
			throw NonexistentElement("Erase of nonexistent");
		eraser(v);						// remove it
	}

	void erase(const Iterator& p)			// remove entry at p
	{
		eraser(p.v);
	}

	void sliceLine(const string &oneline, int &code, int &pop, string &name)
	{
		char delimiter = ',';
		int codeEnd = oneline.find(delimiter);
		string codestr = oneline.substr(0, codeEnd);
		code = std::stoi(codestr.c_str());
		int popEnd = oneline.find(delimiter, codeEnd + 1);
		string popstr = oneline.substr(codeEnd + 1, popEnd);
		pop = std::stoi(popstr.c_str());
		name = oneline.substr(popEnd + 1);
	}

	Iterator begin() {
		TPos v = root();					// start at virtual root
		while (v.isInternal()) v = v.left();		// find leftmost node
		return Iterator(v.parent());
	}

	Iterator end()					// iterator to end entry
	{
		return Iterator(T.root());
	}			// return the super root

	BinaryTree<E> getBinaryTree()
	{
		return T;
	}

protected:						// local utilities
	typedef BinaryTree<E> BinaryTree;			// linked binary tree
	typedef typename BinaryTree::Position TPos;		// position in the tree
	TPos root() const { return T.root().left(); }		// left child of super root

	TPos finder(const K& k, TPos v) {
		if (v.isExternal()) return v;			// key not found
		if (k < (*v).key()) return finder(k, v.left());	// search left subtree
		else if ((*v).key() < k) return finder(k, v.right());	// search right subtree
		else return v;					// found it here
	}

	TPos inserter(const K& k, const V1& x1, const V2& x2) {
		TPos v = finder(k, root());				// search from virtual root
		if (!v.isExternal())					// key already exists?
		{
			(*v).setValue1(x1);  					// replace value1
			(*v).setValue2(x2);
		}
		else
		{
			T.expandExternal(v);				// add new internal node
			(*v).setKey(k); (*v).setValue1(x1);	(*v).setValue2(x2); // set entry
			n++;								// one more entry
		}
		return v;								// return insert position
	}

	TPos eraser(TPos& v) {
		TPos w;
		if (v.left().isExternal()) w = v.left();		// remove from left
		else if (v.right().isExternal()) w = v.right();	// remove from right
		else {						// both internal?
			w = v.right();					// go to right subtree
			do { w = w.left(); } while (!w.isExternal());	// get leftmost node
			TPos u = w.parent();
			(*v).setKey((*u).key()); (*v).setValue1((*u).value1());	(*v).setValue2((*u).value2());// copy w's parent to v
		}
		n--;						// one less entry
		return T.removeAboveExternal(w);			// remove w and parent
	}

	void relink(TPos parent, TPos child, bool makeLeftChild)
	{
		child.setParent(parent);
		if (makeLeftChild)
		{
			parent.setLeft(child);
		}
		else
			parent.setRight(child);
	}

	void rotate(const TPos& p)
	{
		TPos x = p;
		TPos y = x.parent();
		TPos z = y.parent();
		if (z == NULL)
		{
			root() = x;
			x.setParent(NULL);
		}
		else
			relink(z, x, y == z.left());
		// rotate x and y
		if (x == y.left())
		{
			relink(y, x.right(), true);
			relink(x, y, false);
		}
		else
		{
			relink(y, x.left(), false);
			relink(x, y, true);
		}
	}

	TPos restructure(const TPos& v) 
	{
		TPos y = v.parent();
		TPos z = y.parent();
		if ((v == y.right()) == (y == z.right()))
		{
			rotate(y);
			return y;
		}
		else
		{
			rotate(v);
			rotate(v);
			return v;
		}
	} 		

	void inOrder(TPos tp) const
	{
		if (tp != NULL)
		{
			inOrder(tp.left());
			if ((*tp).key() != 0)
			{
				std::cout << (*tp).key() << ", " << (*tp).value1() << ", " << (*tp).value2() << endl;
			}
		}
		
		if (tp != NULL)
			inOrder(tp.right());
	}

		// throw(BoundaryViolation);
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

		bool operator!=(const Iterator& p) const
		{
			return !(*this == p);
		}

		Iterator& operator++() {
			TPos w = v.right();
			if (w.isInternal()) {				// have right subtree?
				do { v = w; w = w.left(); }		// move down left chain
				while (w.isInternal());
			}
			else {
				w = v.parent();					// get parent
				while (v == w.right())			// move up right chain
				{
					v = w; w = w.parent();
				}
				v = w;							// and first link to left
			}
			return *this;
		}

		friend class SearchTree;				// give search tree access
	};
};
#endif