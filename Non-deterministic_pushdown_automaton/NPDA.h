#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
using namespace std;

const int ALPHABET_SIZE = 26;
struct ContextFreeGrammar;
class NPDA
{
	const int MAX_COMPUTATION_STEPS = 50;
	
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

	void ApplyRuleIfPossible(Computation& current, Rule& r, queue<Computation>& q);

	
public:
	NPDA(size_t states);
	NPDA(const ContextFreeGrammar grammar);
	void MakeFinal(size_t ind);
	bool Accepts(const std::string& word, bool shouldPrint = false);
	void AddTransition(int initialState, char symbol, char stackTopSymbol, int destState, string stringToReplaceTopStackSymbol);

};
struct ContextFreeGrammar
{
	std::vector<std::string> grammarRules;
};