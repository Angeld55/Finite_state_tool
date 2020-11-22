#include "NPDA.h"
#include <sstream>

NPDA::NPDA(size_t states) : finalStates(states)
{}

NPDA::NPDA(const ContextFreeGrammar grammar) : NPDA(3)
{
	makeFinal(2);

	addTransition(0, '$', '#', 1, "S#");

	vector<bool> usedSymbols(ALPHABET_SIZE);
	for (int i = 0; i < grammar.grammarRules.size(); i++)
	{
		char var = grammar.grammarRules[i][0];
		std::string rules = grammar.grammarRules[i].substr(3); // removes A->

		istringstream f(rules);
		string rule;

		while (getline(f, rule, '|'))
		{
			for (int i = 0; i < rule.size(); i++)
			{
				if (rule[i] >= 'a' && rule[i] <= 'z')
					usedSymbols[rule[i] - 'a'] = true;
			}
			addTransition(1, '$', var, 1, rule);
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (usedSymbols[i])
			addTransition(1, 'a' + i, 'a' + i, 1, "$");

	addTransition(1, '$', '#', 2, "$");
}

bool NPDA::makeFinal(size_t ind)
{
	if (ind >= finalStates.size())
		return false;
	finalStates[ind] = true;
	return true;
}
void NPDA::addTransition(int initialState, char symbol, char stackTopSymbol, int destState, string stringToReplaceTopStackSymbol)
{
	rules.push_back({ initialState, symbol, stackTopSymbol, destState, stringToReplaceTopStackSymbol });
}
void NPDA::applyRuleIfPossible(Computation& current, Rule& ruleToApply, queue<Computation>& q)
{
	if ((current.state == ruleToApply.initialState) 
		&&((ruleToApply.symbol == '$') || (current.word[0] == ruleToApply.symbol))
		&& (ruleToApply.stackTopSymbol == '$' || current.st.top() == ruleToApply.stackTopSymbol))
	{

		Computation newComputation;

		newComputation.computationSteps = current.computationSteps + 1;
				
		newComputation.state = ruleToApply.destState;
		newComputation.word = ruleToApply.symbol == '$' ? current.word : current.word.substr(1);
		newComputation.st = current.st;

		if (ruleToApply.stackTopSymbol != '$')
			newComputation.st.pop();

		if (ruleToApply.stringToReplaceTopStackSymbol != "$")
		{
			for (int i = ruleToApply.stringToReplaceTopStackSymbol.size() - 1; i >= 0; i--)
				newComputation.st.push(ruleToApply.stringToReplaceTopStackSymbol[i]);
		}
								
		q.push(newComputation);
	}
}
void NPDA::printComputation(const Computation& c)
{
	cout << "State: " << c.state << ", Stack: ";
	stack<char> stCopy = c.st;
	while (!stCopy.empty())
	{
		cout << stCopy.top();
		stCopy.pop();
	}
	cout << ", Word: " << c.word << ", STEPS:" << c.computationSteps << endl;
}

bool NPDA::accepts(const std::string& word, bool shouldPrint )
{
	Computation currentComputation(0, word, 0);
	
	queue<Computation> q;
	currentComputation.st.push('#'); //stack bottom.
	q.push(currentComputation);

	while (!q.empty())
	{
		currentComputation = q.front();
		if (shouldPrint)
			printComputation(currentComputation);

		if (finalStates[currentComputation.state] && currentComputation.word == "")
			return true;

		if (currentComputation.computationSteps == MAX_COMPUTATION_STEPS)
		{
			q.pop();
			continue;
		}

		for (int i = 0; i < rules.size(); i++)
			applyRuleIfPossible(currentComputation, rules[i], q);

		q.pop();
	}
	return false;

}
std::string NPDA::getString()
{
	std::string res = "States count: " + std::to_string(finalStates.size()) + ", Rules count: " + std::to_string(rules.size()) + "\n";
	return res;
}