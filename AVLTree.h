#pragma once

#include "bst.h"
#include <string>
#include <algorithm>
#include "Entry.h"
#include "AVLEntry.h"
#include <fstream>


class AVLTree : public SearchTree<AVLEntry>
{
private:
	string oneLine;
public:
	AVLTree(string filename);
	void print();
	string getOneLine()
	{
		return oneLine;
	}
protected:
	typedef SearchTree<AVLEntry> ST;
	typedef ST::TPos TPos;
	bool isBalanced(const TPos& v) const;
	int height(const TPos& v) const;
	void setHeight(TPos v);
	TPos tallGrandchild(const TPos& v) const;
	void rebalance(const TPos& v);
public:
	Iterator insert(int k, int x1, string x2);
	void erase(int k); 
	void find(int k);
};

AVLTree::AVLTree(string filename)
{
	ifstream infile;
	infile.open(filename);
	if (!infile)
	{
		cout << "Cannot find file!" << endl;
		return;
	}

	int code;
	int pop;
	string countyState;
	SearchTree<AVLEntry> st;
	while (getline(infile, oneLine))
	{
		sliceLine(oneLine, code, pop, countyState);
		TPos tp = finder(code, root());

		if (tp.isRoot())
		{
			st.getBinaryTree().expandExternal(tp);
			(*tp).setKey(code);
			(*tp).setValue1(pop);
			(*tp).setValue2(countyState);
		}
		if (tp.isInternal())
		{
			(*tp).setKey(code);
			(*tp).setValue1(pop);
			(*tp).setValue2(countyState);
		}
		else
		{
			st.getBinaryTree().expandExternal(tp);
			(*tp).setKey(code);
			(*tp).setValue1(pop);
			(*tp).setValue2(countyState);
			setHeight(tp);
			rebalance(tp);
		}

	}
	
}

void AVLTree::print()
{
	TPos tp = root();
	inOrder(tp);
}

AVLTree::Iterator AVLTree::insert(int k, int x1, string x2)
{
	TPos v = inserter(k, x1, x2);
	setHeight(v);
	restructure(v);
	return Iterator(v);
}

void AVLTree::erase(int k) //throw(NonexistentElement)
{
	TPos v = finder(k, ST::root());
	if (Iterator(v) == ST::end())
		throw NonexistentElement("Record does not exist! Cannot delete!");
	else
	{
		if ((*v).key() == 0)
		{
			cout << "Cannot delete. Record does not exist." << endl;
		}
		else
		{
			cout << "Record: " << (*v).key() << ", " << (*v).value1() << ", " << (*v).value2() << " is deleted." << endl;
			TPos w = eraser(v);
			restructure(w);
		}
	}
}

void AVLTree::find(int k)
{
	TPos v = finder(k, ST::root());
	if (Iterator(v) == ST::end())
		throw NonexistentElement("Cannot find the record!");
	else
	{
		if ((*v).key() == 0)
		{
			cout << "Cannot find record. Record does not exist." << endl;
		}
		else
			cout << "The record: " << (*v).key() << ", " << (*v).value1() << ", " << (*v).value2() << " is found" << endl;
	}

}

bool AVLTree::isBalanced(const TPos& v) const
{
	int bal = height(v.left()) - height(v.right());
	return ((-1 <= bal) && (bal <= 1));
	
}

int AVLTree::height(const TPos& v) const
{
	return (v.isExternal() ? 0 : (*v).height());
}

void AVLTree::setHeight(TPos v)
{
	int hl = height(v.left());
	int hr = height(v.right());
	(*v).setHeight(1 + std::max(hl, hr));
}

AVLTree::TPos AVLTree::tallGrandchild(const TPos& z) const
{
	TPos zl = z.left();
	TPos zr = z.right();
	if (height(zl) >= height(zr))
		if (height(zl.left()) >= height(zl.right()))
			return zl.left();
		else
			return zl.right();
	else
		if (height(zr.right()) >= height(zr.left()))
			return zr.right();
		else
			return zr.left();
}

void AVLTree::rebalance(const TPos& v)
{
	//TPos z = v;
	//while (!(z == ST::root()))
	//{
	//	z = z.parent();
	//	setHeight(z);
	//	if (!isBalanced(z))
	//	{
	//		TPos x = tallGrandchild(z);
	//		z = restructure(x);
	//		setHeight(z.left());
	//		setHeight(z.right());
	//		setHeight(z);
	//	}
	//	
	//}
	int oldHeight, newHeight;
	do {
		oldHeight = height(v);
		if (!isBalanced(v))
		{
			restructure(tallGrandchild(v));
			setHeight(v.left());
			setHeight(v.right());
		}
		setHeight(v);
		newHeight = height(v);
		v.parent();
	} while(oldHeight != newHeight && v != NULL);
}
