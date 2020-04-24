#ifndef ENTRY_H
#define ENTRY_H
// Modified for CSCI 220 Fall 15

template <typename K, typename V1, typename V2>
class Entry {					// a (key, value1) pair
public:						// public functions
	typedef K Key;				// key type
	typedef V1 Value1;			// value1 type
	typedef V2 Value2;
	Entry(const K& k = K(), const V1& v1 = V1(), const V2& v2 = V2())	// constructor
		: _key(k), _value1(v1), _value2(v2) { }
	const K& key() const { return _key; }		// get key
	const V1& value1() const { return _value1; }	// get value1
	const V2& value2() const { return _value2; }
	void setKey(const K& k) { _key = k; }		// set key
	void setValue1(const V1& v) { _value1 = v; }	// set value1
	void setValue2(const V2& v) { _value2 = v; }
private:						// private data
	K _key;						// key
	V1 _value1;					// value1
	V2 _value2;
};
#endif