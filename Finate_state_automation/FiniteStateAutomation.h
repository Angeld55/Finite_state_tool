#include<iostream>

#ifndef AUTOMATION_HDR
#define AUTOMATION_HDR

#include "../Automation_base/AutomationBase.h"
#include "../Collections/CustomSet/CustomSet.hpp"




const int ENGLISH_ALPHABET_SIZE = 26;
const int ALPHABET_MAXSIZE = 62;
const int SMALL_A_INDEX = 97;
const int ZERO_INDEX = 48;


class FiniteStateAutomation : public AutomationBase
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

	CustomSet<int> finalStates;
	std::vector<std::vector<edge>> automation;
	CustomSet<char>  alphabet;

	void addTransition(int start, int end, char ch);

public:
	FiniteStateAutomation();
	FiniteStateAutomation(int statesCount);
	FiniteStateAutomation(const std::string& reg);


	//control
	void addLetterToAlphabet(char ch);
	int addState() override;
	int addTransition(const std::vector<std::string>& args) override; //adds an edge between two states
	bool changeStartState(int state);
	int makeStateFinal(size_t state) override;
	void removeState(int state);
	void removeNotReachable();
	bool accepts(const std::string& word, std::string& computation, bool shouldReturnComputation = false) const override; //returns true if automation accepts the string
	bool isEmptyLanguage();
	

	//operations
	friend FiniteStateAutomation Union(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a);

	friend FiniteStateAutomation Complement(const FiniteStateAutomation& a);
	friend FiniteStateAutomation InterSection(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation Reverse(const FiniteStateAutomation& a);

	void reverse();
	
	bool isDeterministic() const;
	bool isTotal() const;

	void makeTotal();
	void makeDeterministic();
	void minimize();

	std::string getRegEx(); //kleeny theorem

	int getStatesCount() const;
	int getStartState() const;
	const CustomSet<int>& getFinalStates() const;

	std::string getString() const override;
	std::string getFullString() const;

	std::string getVisualizeString() const override;

private:

	bool existState(int state); //check if a state exists
	void absorb(const FiniteStateAutomation& a);
	void copyTransitions(int x, int y); 
	CustomSet<int> havePathTo(int begin, const std::string& str) const; //returns set of states reachable with that word

	//for determinstisation
	std::vector<int> getNotReachableStates(int from);
	void DFS(int state, bool* visited);
	FiniteStateAutomation reverseTransitions();
	CustomSet<int> getTransitions(int start, char ch);
	CustomSet<int> getTransitions(const CustomSet<int>&, char ch);
	
	//for kleeny theorem
	std::string getRegEx(int start, int end, int bound, bool needEpsilon);

	//for making total
	int addErrorState();

	//for minimal autoamta (for Brzozowski theorem).
	bool makingMinimal = false;
	CustomSet<int> starts; //simulates multiple starting states for Brzozowski theorem
	bool shouldStartBeFinal;

	void removeNotReachable(int from);

	AutomationBase* clone() const override;

};

FiniteStateAutomation BuildFiniteStateAutomation(const std::string& reg);
FiniteStateAutomation CreateBaseFiniteStateAutomation(char ch); //automation with two states and one letter

#endif // !AUTOMATION_HDR
