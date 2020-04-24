#pragma once

#include "Entry.h"
#include <string>


class AVLEntry : public Entry<int, int, std::string>
{
private:
	int ht;
protected:
	typedef Entry::Key K;
	typedef Entry::Value1 V1;
	typedef Entry::Value2 V2;
	int height() const { return ht; }
	void setHeight(int h) { ht = h; }
public:
	AVLEntry(const K& k = K(), const V1& v1 = V1(), const V2& v2 = V2())
		: Entry(k,v1,v2), ht(0) {}
	friend class AVLTree;
};