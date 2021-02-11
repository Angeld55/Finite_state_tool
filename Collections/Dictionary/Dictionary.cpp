#include "Dictionary.h"

Dictionary::Dictionary()
{
	hashTable = new Node*[N];
	for (int i = 0; i < N; i++)
		hashTable[i] = nullptr;
}
void Dictionary::insertBegin(Set<int> key, int value, int index)
{
	Node* newNode = new Node(key, value);
	newNode->next = hashTable[index];
	hashTable[index] = newNode;
}
Node* Dictionary::getNodeByKey(Set<int> key, int index){
	Node* iter = hashTable[index];
	while (iter != nullptr)
	{
		if (iter->key.Equals(key))
			return iter;
		iter = iter->next;
	}
	return nullptr;
}

unsigned Dictionary::hashFunction(Set<int> key)
{
	int a = 0;
	for (int i = 0; i < key.getSize(); ++i)
		a += key.getAt(i);
	return a;
	
}
bool Dictionary::put(Set<int> key, int value)
{
	
	unsigned result = hashFunction(key);
	if (getNodeByKey(key, result) != nullptr)
		return false; //exists
	insertBegin(key, value, result);
	return true;

}
bool Dictionary::remove(Set<int> key)
{
	unsigned result = hashFunction(key);
	Node* Iter = hashTable[result];
	if (Iter == nullptr)
		return false;
	if (Iter->next == nullptr)
	{
		if (Iter->key.Equals(key))
		{
			delete Iter;
			hashTable[result] = nullptr;
			return true;
		}
		return false;

	}
	Node* prev = Iter;
	Iter = prev->next;

	while (Iter != nullptr)
	{
		if (Iter->key.Equals(key))
		{

			prev->next = Iter->next;
			delete Iter;
			return true;
		}

	}
	return false;

}
int Dictionary::get(Set<int> searched)
{
	unsigned result = hashFunction(searched);
	Node* iter = hashTable[result];
	while (iter)
	{
		if (iter->key.Equals(searched))
			return iter->value;
		iter = iter->next;
	}
	return -1;/// stupid

}

Dictionary::~Dictionary()
{
	for (int i = 0; i < N; ++i)
	{
		Node* iter = hashTable[i];
		while (iter!=nullptr)
		{
			Node* temp = iter;
			iter = iter->next;
			delete temp;
		}
	}
	delete[] hashTable;
}
