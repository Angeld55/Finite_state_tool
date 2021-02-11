#pragma once
#include <iostream>
#include "../Automation_base/AutomationBase.h"
#include <vector>
#include <stack>
#include <queue>
#include <string>
using namespace std;

const int ALPHABET_SIZE = 26;
const int MAX_COMPUTATION_STEPS = 20;
struct ContextFreeGrammar;
class NPDA : public AutomationBase
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

		std::string computationHistory;

	};

	vector<Rule> rules;
	vector<bool> finalStates;

	bool applyRuleIfPossible(Computation& current, const Rule& r, queue<Computation>& q) const;
	bool addTransition(int initialState, char symbol, char stackTopSymbol, int destState, string stringToReplaceTopStackSymbol);

	std::string computationToString(const Computation& c) const;
public:
	NPDA(size_t states = 1);
	NPDA(const ContextFreeGrammar& grammar);
	bool makeStateFinal(size_t ind) override;
	bool accepts(const std::string& word, std::string& computation, bool shouldReturnComputation = false) const override;
	int addState() override;
	int addTransition(const std::vector<std::string>& args) override;
	std::string getString() const override;

	std::string getVisualizeString() const override;
		
    AutomationBase* clone() const override;

};
struct ContextFreeGrammar
{
	std::vector<std::string> grammarRules;
};