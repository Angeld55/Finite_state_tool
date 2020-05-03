#ifndef DICTIONARY_HDR
#define DICTIONARY_HDR

#include "Set.hpp"
#include<iostream>

#define N 221
struct Node
{
	Node(Set<int> key, unsigned value)
	{
		this->key = key;
		this->value = value;
		next = nullptr;
	}
	
	Set<int> key;
	unsigned value;
	Node* next;
};
class Dictionary
{

private:
	Node** hashTable;
	void insertBegin(Set<int> key, int value, int index);
	Node* getNodeByKey(Set<int> key, int index);
	unsigned hashFunction(Set<int> key);
public:
	Dictionary();
	bool remove(Set<int>);
	bool put(Set<int>, int value);
	int get(Set<int>);
	~Dictionary();
};
#endif