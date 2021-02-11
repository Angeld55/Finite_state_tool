#include "FiniteStateAutomation.h"
#include <algorithm>
#include <stack>
#include <string>
#include <cctype>
#include <queue>

//definitions
FiniteStateAutomation::FiniteStateAutomation() : AutomationBase(AutomationBase::AutomationType::FSA)
{
	startState = 0;
	addState();
}

FiniteStateAutomation::FiniteStateAutomation(int statesCount) :AutomationBase(AutomationBase::AutomationType::FSA), automation(statesCount)
{
	startState = 0;
	for (int i = 0; i < statesCount; ++i)
	{
		std::vector<edge> t;
		automation.push_back(t);
	}
}

FiniteStateAutomation::FiniteStateAutomation(const std::string& reg) : AutomationBase(AutomationBase::AutomationType::FSA)
{
	*this = BuildFiniteStateAutomation(reg);
}

void FiniteStateAutomation::addLetterToAlphabet(char ch)
{
	alphabet.Add(ch);
}

int FiniteStateAutomation::addState()
{
	std::vector<edge> t;
	automation.push_back(t);
	return automation.size() - 1;
}

bool FiniteStateAutomation::changeStartState(int state)
{
	if (!existState(state))
		return false;
	startState = state;
	return true;
}

bool FiniteStateAutomation::makeStateFinal(size_t state)
{
	if (!existState(state))
		return false;
	return finalStates.Add(state);
}

int FiniteStateAutomation::getStatesCount() const
{
	return automation.size();
}

int FiniteStateAutomation::getStartState() const
{
	return startState;
}

Set<int> FiniteStateAutomation::getFinalStates() const
{
	return finalStates;
}

bool isNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

int validateInput(const std::vector<std::string>& args)
{
	if (args.size() != 3)
		return -1;
	if (!isNumber(args[0]) || !isNumber(args[1]))
		return -2;
	if (args[2].size() != 1)
		return -1;
	return 0;

}
int FiniteStateAutomation::addTransition(const std::vector<std::string>& args)
{
	int resCode = validateInput(args);

	if (resCode != 0)
		return resCode;

	int start = atoi(args[0].c_str());
	int end = atoi(args[1].c_str());
	char ch = args[2][0];

	if (!existState(start) || !existState(end))
		return -3;

	addTransition(start, end, ch);
}
void FiniteStateAutomation::addTransition(int start, int end, char ch)
{
	edge e(end, ch);
	automation[start].push_back(e);
	alphabet.Add(ch);
}
bool FiniteStateAutomation::existState(int state)
{
	return state < automation.size();
}

Set<int> FiniteStateAutomation::havePathTo(int begin,const std::string& str) const
{
	Set<int> result;

	if (str.length() == 0)
	{
		result.Add(begin);
		return result;
	}
	char firstCh = str[0];

	for (int i = 0; i < automation[begin].size(); i++)
	{
		int otherState = automation[begin][i].dest;
		char symb = automation[begin][i].ch;

		if (symb == firstCh)
		{
			Set<int> resultStates = havePathTo(otherState, str.substr(1));
			result = Union(result, resultStates);
		}
	}
	return result;
}

void FiniteStateAutomation::removeNotReachable(int from)
{
	std::vector<int> notR = getNotReachableStates(from);
	std::sort(notR.begin(), notR.end());
	for (int i = notR.size() - 1; i >= 0; --i)
		removeState(notR[i]);
}

std::vector<int> FiniteStateAutomation::getNotReachableStates(int from)
{
	bool* temp = new bool[automation.size()];
	for (int i = 0; i < automation.size(); ++i)
		temp[i] = false;

	DFS(from, temp);

	std::vector<int> unvisited;
	for (int i = 0; i < automation.size(); ++i)
	{
		if (!temp[i])
			unvisited.push_back(i);

	}
	delete[] temp;
	return unvisited;

}

void FiniteStateAutomation::DFS(int state, bool* visited)
{

	visited[state] = true;
	for (int i = 0; i < automation[state].size(); ++i)
	{
		if (!visited[automation[state][i].dest])
			DFS(automation[state][i].dest, visited);
	}
}

FiniteStateAutomation FiniteStateAutomation::reverseTransitions()
{
	FiniteStateAutomation result(automation.size());
	result.alphabet = alphabet;

	//reverse all the transition
	for (int i = 0; i < automation.size(); ++i)
	{
		for (int j = 0; j < automation[i].size(); ++j)
		{
			edge current = automation[i][j];
			result.addTransition(current.dest, i, current.ch);
		}
	}
	return result;

}

Set<int> FiniteStateAutomation::getTransitions(int start, char ch)
{
	Set<int> result;
	for (int i = 0; i < automation[start].size(); ++i)
	{
		if (automation[start][i].ch == ch)
			result.Add(automation[start][i].dest);
	}
	return result;
}

Set<int> FiniteStateAutomation::getTransitions(const Set<int>& s, char ch)
{
	Set<int> result;
	for (int i = 0; i < s.getSize(); ++i)
		result = Union(result, getTransitions(s.getAt(i), ch));
	return result;
}

void FiniteStateAutomation::removeState(int state)
{
	if (state == startState)
		return;
	automation.erase(automation.begin() + state);
	for (int i = 0; i < automation.size(); ++i)
	{

		for (int j = 0; j < automation[i].size(); ++j)
		{
			if (automation[i][j].dest>state)
				automation[i][j].dest--;
			else if (automation[i][j].dest == state)//may not be the case if it's unreachable
				automation[i].erase(automation[i].begin() + j--);
		}
	}
	Set<int> fs;
	for (int i = 0; i < finalStates.getSize(); ++i)
	{

		int finalState = finalStates.getAt(i);

		if (finalState == state)
			continue;
		if (finalState > state)
			finalState--;
		fs.Add(finalState);
	}
	finalStates = fs;
}

void FiniteStateAutomation::removeNotReachable()
{
	removeNotReachable(startState);
}

bool FiniteStateAutomation::accepts(const std::string& word, std::string& computation, bool shouldReturnComputation) const
{
	Set<int> result = havePathTo(startState, word);
	Set<int> intersection = Intersection(finalStates, result);

	return intersection.getSize();
}

bool FiniteStateAutomation::isEmptyLanguage()
{
	bool* visited = new bool[automation.size()];
	for (int i = 0; i < automation.size(); i++)
		visited[i] = false;

	std::queue<int> gray;
	gray.push(getStartState());
	visited[getStartState()] = true;

	while (!gray.empty()) {

		int currentVertex = gray.front();
		gray.pop();

		if (getFinalStates().Contains(currentVertex))
		{
			delete[] visited;
			return false;
		}

		for (int i = 0; i < automation[currentVertex].size(); i++)
		{
			int neighbor = automation[currentVertex][i].dest;
			if (!visited[neighbor])
			{
				visited[neighbor] = true;
				gray.push(neighbor);
			}
		}
	}
	delete[] visited;
	return true;
}
bool  FiniteStateAutomation::isTotal() const
{
	bool* temp = new bool[alphabet.getSize()];
	int errorStateIndex = -1;
	for (int i = 0; i < automation.size(); ++i)
	{
		for (int j = 0; j < alphabet.getSize(); ++j)
			temp[j] = false;
		for (int j = 0; j < automation[i].size(); ++j)
			temp[alphabet.IndexOf(automation[i][j].ch)] = true; 
		for (int j = 0; j < alphabet.getSize(); ++j)
		{
			if (!temp[j])
			{
				delete[] temp;
				return false;
			}
		}
		for (int i = 0; i < alphabet.getSize(); ++i)
			temp[i] = false;

	}
	delete[] temp;
	return true;
}
void FiniteStateAutomation::makeTotal()
{
	bool* temp = new bool[alphabet.getSize()];
	int errorStateIndex = -1;
	for (int i = 0; i < automation.size(); ++i)
	{
		for (int j = 0; j < alphabet.getSize(); ++j) //we mark all letters as not-used
			temp[j] = false;
		for (int j = 0; j < automation[i].size(); ++j)
			temp[alphabet.IndexOf(automation[i][j].ch)] = true; //may not be the best implementation. For big set of letter would be slow
		for (int j = 0; j < alphabet.getSize(); ++j) //we check with which of the letters we don't have a transition
		{
			if (!temp[j])
			{
				if (errorStateIndex == -1)
					errorStateIndex = addErrorState();
				addTransition(i, errorStateIndex, alphabet.getAt(j));
			}
		}
		for (int i = 0; i < alphabet.getSize(); ++i) //we mark all letters as not-used AGAIN (for the next state)
			temp[i] = false;

	}
	delete[] temp;
}

bool FiniteStateAutomation::isDeterministic() const
{
	for (int i = 0; i < automation.size(); i++)
	{
		for (int j = 0; j < automation[i].size(); j++)
		{
			for (int k = j + 1; k < automation[i].size(); k++)
			{
				if (automation[i][j].ch == automation[i][k].ch)
					return false;
			}
		}
	}
	return true;
}

void FiniteStateAutomation::absorb(const FiniteStateAutomation& a)
{
	int sizeFiniteStateAutomation = automation.size();

	for (int k = 0; k < a.automation.size(); k++)
	{
		automation.push_back(a.automation[k]);

		for (int j = 0; j < a.automation[k].size(); j++)
			automation[k + sizeFiniteStateAutomation][j].changeDest(sizeFiniteStateAutomation + automation[k + sizeFiniteStateAutomation][j].dest);

		if (a.finalStates.Contains(k))
			finalStates.Add(sizeFiniteStateAutomation + k);
	}
}

void FiniteStateAutomation::copyTransitions(int x, int y)
{
	int to = automation[y].size();

	for (int i = 0; i < to; i++)
		automation[x].push_back(automation[y][i]);
}

std::string FiniteStateAutomation::getString() const
{
	std::string res = "Deterministic: ";
	res += isDeterministic() ? "True, " : "False, ";
	res += "Total: ";
	res += isTotal() ? "True, " : "False, ";
	res += "Letters: {";
	for (int i = 0; i < alphabet.getSize(); i++)
	{
		res += alphabet.getAt(i);
		if (i != alphabet.getSize() - 1)
			res += ", ";
	
	}
	res += ("}, States count: " + std::to_string(automation.size()) + ".\n");
	return res;
}

std::string FiniteStateAutomation::getFullString() const
{
	std::string res;
	for (int i = 0; i < automation.size(); i++)
	{
		res += "State: " + std::to_string(i);
		if (i == startState)
			res += "(S)";
		else
			res += "    ";
		if (finalStates.Contains(i))
			res += "(F)";
		else
			res += "   ";
		res += " transitions: ";
		for (int j = 0; j < automation[i].size(); j++)
			res =  res + "with " + automation[i][j].ch + " to " + std::to_string(automation[i][j].dest) + "   ";
		if (i != automation.size()-1)
			res += '\n';
	}

	return res;
}

std::string getFinalStatesString(const Set<int>& finalStates)
{
	std::string res = "node[shape = doublecircle] ";
	for (int i = 0; i < finalStates.getSize(); i++)
		res += std::to_string(finalStates.getAt(i)) + " ";
	return res;
}
std::string FiniteStateAutomation::getVisualizeString() const
{
	
	std::string str = "\n digraph finite_state_machine \n { \n";
	str += "\trankdir = LR; \n \tsize = \"8,5\" \n";
	str += "node [shape=point]";
	str += std::to_string(automation.size()) + ";\n" + getFinalStatesString(finalStates) + "; \n\tnode [shape = circle]; \n";


	str += '\t' + std::to_string(automation.size()) + " -> " + std::to_string(startState) + "\n";
	//transitions
	for (int i = 0; i < automation.size(); i++)
	{
		for (int j = 0; j < automation[i].size(); j++)
		{
			str += '\t' + std::to_string(i) + " -> " + std::to_string(automation[i][j].dest) + "[" + "label = \"" + ((char)automation[i][j].ch) + "\"" + "]\n";
		}
	}
	str += "}";
	return str;
}

void FiniteStateAutomation::makeDeterministic()
{
	FiniteStateAutomation result;
	result.alphabet = alphabet;
	result.makingMinimal = makingMinimal;
	std::queue<Set<int>> newStates;
	Dictionary newStatesIndecies;

	//init start state

	Set<int> newStartState;

	if (!makingMinimal)
	{
		newStartState.Add(startState);
		if (getFinalStates().Contains(startState))
			result.makeStateFinal(0);
	}
	else
	{
		newStartState = starts;
		if (shouldStartBeFinal)
			result.makeStateFinal(0);
	}
	newStatesIndecies.put(newStartState, 0);
	newStates.push(newStartState);

	int statesCount = 1;
	while (!newStates.empty())
	{
		Set<int> currentState = newStates.front();
		newStates.pop();


		int currentStateIndex = newStatesIndecies.get(currentState);


		for (int i = 0; i < alphabet.getSize(); ++i)
		{
			Set<int> currentStateSet = getTransitions(currentState, alphabet.getAt(i));
			if (currentStateSet.getSize() == 0)
				continue;
			if (newStatesIndecies.get(currentStateSet) != -1) //if the state exists
			{
				int destStateIndex = newStatesIndecies.get(currentStateSet);
				result.addTransition(currentStateIndex, destStateIndex, alphabet.getAt(i));
			}
			else
			{
				result.addState();
				newStatesIndecies.put(currentStateSet, statesCount++);
				newStates.push(currentStateSet);

				if (Intersection(currentStateSet, finalStates).getSize() != 0)
					result.makeStateFinal(statesCount - 1);

				result.addTransition(currentStateIndex, statesCount - 1, alphabet.getAt(i));
			}
		}
	}
	result.makeTotal();
	*this = result;
	//print();
}

void FiniteStateAutomation::reverse()
{
	FiniteStateAutomation result = reverseTransitions();
	result.makeStateFinal(startState);
	if (makingMinimal)
	{
		Set<int> temp = finalStates;
		bool temp2 = false;
		if (finalStates.Contains(startState))
			temp2 = true;
		result.starts = temp;
		result.shouldStartBeFinal = temp2;
		result.makingMinimal = true;
		*this = result;
		return;
	}

	//new start state
	int newStart = result.addState();
	result.changeStartState(newStart);

	//the new start state is final if the old start state was final.
	if (getFinalStates().Contains(startState))
		result.makeStateFinal(newStart);

	//add the transiotions to the new start state.
	for (int i = 0; i < finalStates.getSize(); ++i)
	{
		int currentFinalState = finalStates.getAt(i);
		for (int j = 0; j < result.automation[currentFinalState].size(); ++j)
		{
			edge current = result.automation[currentFinalState][j];
			result.addTransition(newStart, current.dest, current.ch);
		}
	}
	*this = result;

}

void FiniteStateAutomation::minimize()
{

	makeDeterministic();
	makingMinimal = true;
	reverse();
	makeDeterministic();
	reverse();
	makeDeterministic();
	makingMinimal = false;

}

std::string FiniteStateAutomation::getRegEx()
{
	std::string res = "";
	for (int i = 0; i < finalStates.getSize(); ++i)
	{
		if (i > 0)
			res += '+';
		res += "[ " + getRegEx(startState, finalStates.getAt(i), automation.size(), true) + " ]";
	}
	return res;
}

FiniteStateAutomation Union(const FiniteStateAutomation& a, const FiniteStateAutomation& b)
{ 
	FiniteStateAutomation result = a;
	result.absorb(b);
	result.addState(); //it will be the new start state
	result.changeStartState(result.getStatesCount() - 1);
	result.copyTransitions(result.getStatesCount() - 1, a.getStartState());
	result.copyTransitions(result.getStatesCount() - 1, a.getStatesCount() + b.getStartState());

	if (a.getFinalStates().Contains(a.getStartState()) || b.getFinalStates().Contains(b.getStartState()))
		result.makeStateFinal(result.getStartState());
	result.alphabet = Union(a.alphabet, b.alphabet);
	return result;
}

FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b)
{
	FiniteStateAutomation result = a;
	Set<int> firstFiniteStateAutomationFinalStats(a.getFinalStates()); // copy c-tor
	result.absorb(b);

	for (int i = 0; i < firstFiniteStateAutomationFinalStats.getSize(); i++)
		result.copyTransitions(firstFiniteStateAutomationFinalStats.getElement(i), b.getStartState() + a.getStatesCount());

	if (!b.getFinalStates().Contains(b.getStartState()))
		result.finalStates = SetDifference(result.finalStates, firstFiniteStateAutomationFinalStats);
	result.alphabet = Union(a.alphabet, b.alphabet);
	return result;
}

FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a)
{
	FiniteStateAutomation result = a;
	result.addState();
	result.changeStartState(result.getStatesCount() - 1);
	result.copyTransitions(result.getStatesCount() - 1, a.getStartState());

	for (int i = 0; i < result.getFinalStates().getSize(); i++)
		result.copyTransitions(result.getFinalStates().getElement(i), result.getStartState());
	result.makeStateFinal(result.getStartState());

	return result;
}

FiniteStateAutomation Complement(const FiniteStateAutomation& a)
{

	FiniteStateAutomation result = a;
	result.makeDeterministic();
	result.makeTotal();
	Set<int> newFinals;
	for (int i = 0; i < result.automation.size(); ++i)
	{
		if (!result.finalStates.Contains(i))
			newFinals.Add(i);
	}
	result.finalStates = newFinals;
	return result;
}

FiniteStateAutomation InterSection(const FiniteStateAutomation& a, const FiniteStateAutomation& b)
{
	return Complement(Union(Complement(a), Complement(b))); // De Morgan's Laws
}

FiniteStateAutomation Reverse(const FiniteStateAutomation& a);

bool isSymbol(char ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '$';
}
int getClosingBracketIndex(const std::string& reg)
{
	int count = 1;
	for (int i = 1; i < reg.length(); i++)
	{
		if (reg[i] == '(')
			count++;
		if (reg[i] == ')')
			count--;
		if (count == 0)
			return i;
	}
	return -1; //error
}
// abc  a b . c .
std::string convertRegexToRPN(const std::string& regex)
{
	std::string result;
	std::stack<char> operatorStack;

	for (int i = 0; i < regex.length(); i++)
	{
		if (isSymbol(regex[i]))
		{
			result += regex[i];
				
		}
		else if (regex[i] == '(')
		{
			operatorStack.push('(');
		}
		else if (regex[i] == ')')
		{
			while (operatorStack.top() != '(')
			{
				result += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop();
		}
		else if (regex[i] == '*')
		{
			result += '*';

		}
		else if (regex[i] == '+')
		{
			while (!operatorStack.empty() && (operatorStack.top() == '.' || operatorStack.top() == '+'))
			{
				result += operatorStack.top();
				operatorStack.pop();
			}

			operatorStack.push(regex[i]);

		}
		if (i != regex.length() - 1)
		{
			char l = regex[i];
			char r = regex[i + 1];
			if ((isSymbol(l) && (isSymbol(r) || r == '(')) || (l == ')' && (isSymbol(r) || r == '(')) ||( l == '*' && (r == '(' || isSymbol(r))))
			{
				while (!operatorStack.empty() && (operatorStack.top() == '.' || operatorStack.top() == '*'))
				{
					result += operatorStack.top();
					operatorStack.pop();
				}

				operatorStack.push('.');
			}
		}
	}
	while (!operatorStack.empty())
	{
		result += operatorStack.top();
		operatorStack.pop();
	}
	return result;
}

FiniteStateAutomation BuildFiniteStateAutomation(const std::string& reg)
{
	std::string regexRPN = convertRegexToRPN(reg);
	std::stack<FiniteStateAutomation> automationStack;

	for (int i = 0; i < regexRPN.length(); i++)
	{
		if (isSymbol(regexRPN[i]))
			automationStack.push(CreateBaseFiniteStateAutomation(regexRPN[i]));
		else if (regexRPN[i] == '+')
		{
			FiniteStateAutomation rhs = automationStack.top();
			automationStack.pop();
			FiniteStateAutomation lhs = automationStack.top();
			automationStack.pop();
			automationStack.push(Union(lhs, rhs));
		}
		else if (regexRPN[i] == '.')
		{
			FiniteStateAutomation rhs = automationStack.top();
			automationStack.pop();
			FiniteStateAutomation lhs = automationStack.top();
			automationStack.pop();
			automationStack.push(Concat(lhs, rhs));
		}
		else if (regexRPN[i] == '*')
		{
			FiniteStateAutomation arg = automationStack.top();
			automationStack.pop();
			automationStack.push(KleeneStar(arg));
		}

	//	if (regexRPN[i] == '')
	}

	return automationStack.top(); //error


}

FiniteStateAutomation CreateBaseFiniteStateAutomation(char ch)
{
	FiniteStateAutomation a;
	if (ch == '$')
	{
		a.makeStateFinal(0);
		return a;
	}
	a.addLetterToAlphabet(ch);
	a.addState();
	std::string letter = std::string(1, ch);
	a.addTransition({ "0", "1", letter });
	a.makeStateFinal(1);
	return a;
}

FiniteStateAutomation Reverse(const FiniteStateAutomation& a)
{
	FiniteStateAutomation res(a);
	res.reverse();
	return res;
}

//if a regex need brackets for better understanding in Auomation to regex conversation.
bool needBrackets(std::string regex)
{
	if (regex.length() == 0)
		return false;
	if (regex[0] == '(' && regex[regex.length() - 1] == ')')
		return false;

	for (int i = 0; i < regex.length(); ++i)
	{
		if (regex[i] == '+')
			return true;
	}
	return regex[regex.length() - 1] == '*';
}

std::string FiniteStateAutomation::getRegEx(int start, int end, int bound, bool needEpsilon)
{
	if (bound == 0)
	{
		Set<char> s;

		if (start == end && needEpsilon)
			s.Add('e');
		for (int i = 0; i < automation[start].size(); ++i)
		{
			edge currentEdge = automation[start][i];
			if (currentEdge.dest == end)
				s.Add(currentEdge.ch);
		}
		std::string str;
		for (int i = 0; i < s.getSize(); ++i)
		{
			if (i != 0)
				str += " + ";
			str += s.getAt(i);
		}
		return str;
	}
	
	std::string left = getRegEx(start, end, bound - 1, needEpsilon);

	std::string b = getRegEx(start, bound - 1, bound - 1, needEpsilon);
	std::string c = getRegEx(bound - 1, bound - 1, bound - 1, false); // Don't need epsilon (we have * -> it contains it)
	std::string d = getRegEx(bound - 1, end, bound - 1, needEpsilon);

	std::string right;
	if (b != "e")
	{
		if (needBrackets(b))
			right += "(" + b + ")";
		else
			right += b;
	}


	if (c != "e" && c != "")
		right += "(" + c + ")*";

	if (d != "e")
	{
		if (needBrackets(d))
			right += "(" + d + ")";
		else
			right += d;
	}



	if (b == "e" && (c == "e" || c == "") && d == "e")
		right = "e";
	if (b == "" || d == "")
		right = "";

	if (left == "" && right == "")
		return "";
	if (left == "")
		return right;
	if (right == "")
		return left;
	if (left == right)
		return left;

	std::string regex = left;
	if (needBrackets(right))
		regex = regex + " + " + "(" + right + ")";
	else
		regex += " + " + right;
	return regex;


}

int FiniteStateAutomation::addErrorState()
{
	int ind = addState();
	for (int i = 0; i < alphabet.getSize(); ++i)
		addTransition(ind, ind, alphabet.getAt(i));
	return ind;
}

AutomationBase* FiniteStateAutomation::clone() const
{
	return new FiniteStateAutomation(*this);
}