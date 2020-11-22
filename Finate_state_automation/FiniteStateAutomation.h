#include<iostream>

#ifndef AUTOMATION_HDR
#define AUTOMATION_HDR

#include "../Collections/Queue.hpp"
#include "../Collections/DynamicArray.hpp"
#include "../Collections/Set.hpp"
#include "../String/String.h"
#include "../Collections/Dictionary.h"




const int ENGLISH_ALPHABET_SIZE = 26;
const int ALPHABET_MAXSIZE = 62;
const int SMALL_A_INDEX = 97;
const int ZERO_INDEX = 48;


class FiniteStateAutomation
{
	struct edge
	{

		int dest;
		char ch;
		edge()
		{
			dest = -1;
			ch = '/0';
		}

		edge(int dest, char ch)
		{
			this->dest = dest;
			this->ch = ch;
		}

		void changeDest(int dest)
		{
			this->dest = dest;
		}
	};


private:
	int startState;

	Set<int> finalStates;
	DynamicArray<DynamicArray<edge>> automation;
	Set<char>  alphabet;


public:
	FiniteStateAutomation();
	FiniteStateAutomation(int statesCount);
	FiniteStateAutomation(const String& reg);


	//control
	void addLetterToAlphabet(char ch);
	int addState();
	bool addTransition(int start, int end, char ch); //adds an edge between two states
	bool changeStartState(int state);
	bool makeStateFinal(int state);
	void removeState(int state);
	void removeNotReachable();
	bool accepts(String str); //returns true if automation accepts the string
	bool isEmptyLanguage();
	



	//operations
	friend FiniteStateAutomation Union(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a);

	friend FiniteStateAutomation Complement(const FiniteStateAutomation& a);
	friend FiniteStateAutomation InterSection(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation Reverse(const FiniteStateAutomation& a);

	void reverse();
	
	bool isDeterministic();
	bool isTotal();

	void makeTotal();
	void makeDeterministic();
	void minimize();

	String getRegEx(); //kleeny theorem

	int getStatesCount() const;
	int getStartState() const;
	Set<int> getFinalStates() const;

	void print();

	std::string getString();

	

private:

	bool existState(int state); //check if a state exists
	void absorb(const FiniteStateAutomation& a);
	void copyTransitions(int x, int y); 
	Set<int> havePathTo(int begin, String str); //returns set of states reachable with that word
	void CheckIfOneStated();

	//for determinstisation
	
	DynamicArray<int> getNotReachableStates(int from);
	void DFS(int state, bool* visited);
	FiniteStateAutomation reverseTransitions();
	Set<int> getTransitions(int start, char ch);
	Set<int> getTransitions(Set<int>, char ch);
	

	//for kleeny theorem
	String getRegEx(int start, int end, int bound, bool needEpsilon);

	//for making total
	int addErrorState();

	//for minimal autoamta (for Brzozowski theorem).
	bool makingMinimal = false;
	Set<int> starts; //simulates multiple starting states for Brzozowski theorem
	bool shouldStartBeFinal;

	void removeNotReachable(int from);

};

FiniteStateAutomation BuildFiniteStateAutomation(String reg);
FiniteStateAutomation CreateBaseFiniteStateAutomation(char ch); //automation with two states and one letter

#endif // !AUTOMATION_HDR
