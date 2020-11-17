#include "NPDA.h"
#include <sstream>

NPDA::NPDA(size_t states) : finalStates(states)
{}

NPDA::NPDA(const ContextFreeGrammar grammar) : NPDA(3)
{
	MakeFinal(2);

	AddTransition(0, '$', '#', 1, "S#");

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
			AddTransition(1, '$', var, 1, rule);
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (usedSymbols[i])
			AddTransition(1, 'a' + i, 'a' + i, 1, "$");

	AddTransition(1, '$', '#', 2, "$");
}

void NPDA::MakeFinal(size_t ind)
{
	if (ind >= finalStates.size())
		return;
	finalStates[ind] = true;
}
void NPDA::AddTransition(int initialState, char symbol, char stackTopSymbol, int destState, string stringToReplaceTopStackSymbol)
{
	rules.push_back({ initialState, symbol, stackTopSymbol, destState, stringToReplaceTopStackSymbol });
}
void PDA::ApplyRuleIfPossible(Computation& current, Rule& ruleToApply, queue<Computation>& q)
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

bool NPDA::Accepts(const std::string& word, bool shouldPrint )
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
			ApplyRuleIfPossible(currentComputation, rules[i], q);

		q.pop();
	}
	return false;

}
