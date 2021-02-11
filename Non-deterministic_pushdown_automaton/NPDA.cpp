#include "NPDA.h"
#include <cctype>
#include <sstream>

NPDA::NPDA(size_t states) : AutomationBase(AutomationBase::AutomationType::NPDA), finalStates(states)
{}

NPDA::NPDA(const ContextFreeGrammar& grammar) :  NPDA(3)
{
	makeStateFinal(2);

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

bool NPDA::makeStateFinal(size_t ind)
{
	if (ind >= finalStates.size())
		return false;
	finalStates[ind] = true;
	return true;
}
int NPDA::addState()
{
	finalStates.push_back(false);
	return finalStates.size() - 1;
}
bool isValidNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
int validateCustomInput(const std::vector<std::string>& args)
{
	if (args.size() != 5)
		return -1;
	if (!isValidNumber(args[0]) || !isValidNumber(args[3]))
		return  -2;

	if (args[1].size() != 1 || args[2].size() != 1)
		return -4;
	return 0;
}
int NPDA::addTransition(const std::vector<std::string>& args)
{
	int resCodevalidateInput = validateCustomInput(args);
	
	if (resCodevalidateInput != 0)
		return resCodevalidateInput;

	resCodevalidateInput = addTransition(atoi(args[0].c_str()), args[1][0], args[2][0], atoi(args[3].c_str()), args[4]);
	if (!resCodevalidateInput)
		return -3;
	return 0;
}

bool NPDA::addTransition(int initialState, char symbol, char stackTopSymbol, int destState, string stringToReplaceTopStackSymbol)
{
	if (initialState >= finalStates.size() || destState >= finalStates.size())
		return false;

	rules.push_back({ initialState, symbol, stackTopSymbol, destState, stringToReplaceTopStackSymbol });
	return true;
}
bool NPDA::applyRuleIfPossible(Computation& current, const Rule& ruleToApply, queue<Computation>& q) const
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
		return true;
	}
	return false;
}
std::string NPDA::computationToString(const Computation& c)  const
{
	std::string res =  "< State: " + std::to_string(c.state) + ", Stack: ";
	stack<char> stCopy = c.st;
	while (!stCopy.empty())
	{
		res += stCopy.top();
		stCopy.pop();
	}
	return res += ", Word: " + c.word + ", STEP:" + std::to_string(c.computationSteps) + " > \n";
}

bool NPDA::accepts(const std::string& word, std::string& computationHistory, bool shouldReturnComputation) const
{
	Computation currentComputation(0, word, 0);
	
	queue<Computation> q;
	currentComputation.st.push('#'); //stack bottom.
	q.push(currentComputation);

	while (!q.empty())
	{
		currentComputation = q.front();

		if (finalStates[currentComputation.state] && currentComputation.word == "")
		{
			computationHistory = currentComputation.computationHistory;
			return true;
		}
		if (currentComputation.computationSteps == MAX_COMPUTATION_STEPS)
		{
			q.pop();
			continue;
		}

		for (int i = 0; i < rules.size(); i++)
		{
			if (applyRuleIfPossible(currentComputation, rules[i], q) && shouldReturnComputation)
			{
				Computation& lastAdded = q.back();
				lastAdded.computationHistory = currentComputation.computationHistory + computationToString(currentComputation);
			}
		}

		q.pop();
	}
	return false;

}
std::string NPDA::getString() const
{
	std::string res = "States count: " + std::to_string(finalStates.size()) + ", Rules count: " + std::to_string(rules.size()) + "\n";
	return res;
}

std::string  NPDA::getVisualizeString() const
{
	//TBI
	return "";
}
AutomationBase* NPDA::clone() const
{
	return new NPDA(*this);
}