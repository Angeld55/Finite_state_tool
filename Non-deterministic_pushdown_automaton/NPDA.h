#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
using namespace std;

const int ALPHABET_SIZE = 26;
const int MAX_COMPUTATION_STEPS = 50;
struct ContextFreeGrammar;
class NPDA
{

	struct Rule
	{
		int initialState;
		char symbol;
		char stackTopSymbol;
		int destState;
		string stringToReplaceTopStackSymbol;
	};

	struct Computation
	{
		Computation() :computationSteps(0) {}
		Computation(int state, string word, int computationSteps) :
			state(state), word(word), computationSteps(computationSteps)
		{}

		int state;
		stack<char> st;
		string word;
		int computationSteps; 

	};

	void printComputation(const Computation& c);

	vector<Rule> rules;
	vector<bool> finalStates;

	void applyRuleIfPossible(Computation& current, Rule& r, queue<Computation>& q);

	
public:
	NPDA(size_t states = 1);
	NPDA(const ContextFreeGrammar grammar);
	bool makeFinal(size_t ind);
	bool accepts(const std::string& word, bool shouldPrint = false);
	int addState();
	bool addTransition(int initialState, char symbol, char stackTopSymbol, int destState, string stringToReplaceTopStackSymbol);
	std::string getString();

};
struct ContextFreeGrammar
{
	std::vector<std::string> grammarRules;
};