#include "FiniteStateAutomation.h"
#include <algorithm>
#include <stack>
#include <string>
#include <cctype>
#include <queue>
#include <map>

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
	alphabet.add(ch);
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

int FiniteStateAutomation::makeStateFinal(size_t state)
{
	if (!existState(state))
		return -2;
	return finalStates.add(state) ? 0 : -1;
}

int FiniteStateAutomation::getStatesCount() const
{
	return automation.size();
}

int FiniteStateAutomation::getStartState() const
{
	return startState;
}

const CustomSet<int>& FiniteStateAutomation::getFinalStates() const
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

	return 0;
}

void FiniteStateAutomation::addTransition(int start, int end, char ch)
{
	edge e(end, ch);
	automation[start].push_back(e);
	alphabet.add(ch);
}

bool FiniteStateAutomation::existState(int state)
{
	return state < automation.size();
}

CustomSet<int> FiniteStateAutomation::havePathTo(int begin, const std::string& str) const
{
	CustomSet<int> result;

	if (str.length() == 0)
	{
		result.add(begin);
		return result;
	}
	char firstCh = str[0];

	for (int i = 0; i < automation[begin].size(); i++)
	{
		int otherState = automation[begin][i].dest;
		char symb = automation[begin][i].ch;

		if (symb == firstCh)
		{
			CustomSet<int> resultStates = havePathTo(otherState, str.substr(1));
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

CustomSet<int> FiniteStateAutomation::getTransitions(int start, char ch)
{
	CustomSet<int> result;
	for (int i = 0; i < automation[start].size(); ++i)
	{
		if (automation[start][i].ch == ch)
			result.add(automation[start][i].dest);
	}
	return result;
}

CustomSet<int> FiniteStateAutomation::getTransitions(const CustomSet<int>& s, char ch)
{
	CustomSet<int> result;

	for (auto it = s.getRaw().begin(); it != s.getRaw().end(); it++)
		result = Union(result, getTransitions(*it, ch));

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
	CustomSet<int> fs;

	for (auto it = finalStates.getRaw().begin(); it != finalStates.getRaw().end(); it++)
	{
		int finalState = *it;

		if (finalState == state)
			continue;
		if (finalState > state)
			finalState--;
		fs.add(finalState);
	}

	finalStates = fs;
}

void FiniteStateAutomation::removeNotReachable()
{
	removeNotReachable(startState);
}

bool FiniteStateAutomation::accepts(const std::string& word, std::string& computation, bool shouldReturnComputation) const
{
	CustomSet<int> result = havePathTo(startState, word);
	CustomSet<int> intersection = Intersection(finalStates, result);

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

		if (getFinalStates().contains(currentVertex))
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
bool FiniteStateAutomation::isTotal() const
{
	std::set<char> temp;

	int errorStateIndex = -1;
	for (int i = 0; i < automation.size(); ++i)
	{
		temp.clear();
		
		for (int j = 0; j < automation[i].size(); ++j)
			temp.insert(automation[i][j].ch);
	
		if (temp != alphabet.getRaw())
			return false;

	}
	return true;
}

void FiniteStateAutomation::makeTotal()
{
	std::set<char> temp;
	int errorStateIndex = -1;
	for (int i = 0; i < automation.size(); ++i)
	{
		temp.clear();

		for (int j = 0; j < automation[i].size(); ++j)
			temp.insert(automation[i][j].ch); 
		
		int j = 0;
		for (auto it = alphabet.getRaw().begin(); it != alphabet.getRaw().end(); it++, j++)
		{
			auto findIt = temp.find(*it);
			if (findIt == temp.end())
			{
				if (errorStateIndex == -1)
					errorStateIndex = addErrorState();
				addTransition(i, errorStateIndex, *it);
			}
		}
	}

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

		if (a.finalStates.contains(k))
			finalStates.add(sizeFiniteStateAutomation + k);
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
	
	for (auto it = alphabet.getRaw().begin(); it != alphabet.getRaw().end(); it++)
	{
		if (it != alphabet.getRaw().begin())
			res += ", ";

		res += *it;
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
		if (finalStates.contains(i))
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

std::string getFinalStatesString(const CustomSet<int>& finalStates)
{
	std::string res = "node[shape = doublecircle] ";

	for (auto it = finalStates.getRaw().begin(); it != finalStates.getRaw().end(); it++)
		res += std::to_string(*it) + " ";

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
		if (automation[i].size() == 0)
		{
			str += '\t' + std::to_string(i) + '\n';
		}
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
	std::queue<CustomSet<int>> newStates;
	std::map<CustomSet<int>, int> newStatesIndecies;
	//Dictionary newStatesIndecies;

	//init start state

	CustomSet<int> newStartState;

	if (!makingMinimal)
	{
		newStartState.add(startState);
		if (getFinalStates().contains(startState))
			result.makeStateFinal(0);
	}
	else
	{
		newStartState = starts;
		if (shouldStartBeFinal)
			result.makeStateFinal(0);
	}
	newStatesIndecies.insert(std::make_pair(newStartState, 0));
	newStates.push(newStartState);

	int statesCount = 1;
	while (!newStates.empty())
	{
		CustomSet<int> currentState = newStates.front();
		newStates.pop();

		int currentStateIndex = (*newStatesIndecies.find(currentState)).second;

		for (auto it = alphabet.getRaw().begin(); it != alphabet.getRaw().end(); it++)
		{
			CustomSet<int> currentStateSet = getTransitions(currentState, (*it));
			if (currentStateSet.getSize() == 0)
				continue;

			auto itNewStateIndecies = newStatesIndecies.find(currentStateSet);
			if (itNewStateIndecies != newStatesIndecies.end()) //if the state exists
			{
				int destStateIndex = (*itNewStateIndecies).second;
				result.addTransition(currentStateIndex, destStateIndex, (*it));
			}
			else
			{
				result.addState();
				newStatesIndecies.insert(std::make_pair(currentStateSet, statesCount++));
				newStates.push(currentStateSet);

				if (Intersection(currentStateSet, finalStates).getSize() != 0)
					result.makeStateFinal(statesCount - 1);

				result.addTransition(currentStateIndex, statesCount - 1, (*it));
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
		CustomSet<int> temp = finalStates;
		bool temp2 = false;
		if (finalStates.contains(startState))
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
	if (getFinalStates().contains(startState))
		result.makeStateFinal(newStart);

	//add the transiotions to the new start state.
	for (auto it = finalStates.getRaw().begin(); it != finalStates.getRaw().end(); it++)
	{
		int currentFinalState = (*it);
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
	for (auto it = finalStates.getRaw().begin(); it != finalStates.getRaw().end(); it++)
	{
		if (it != finalStates.getRaw().begin())
			res += '+';
		res += "[ " + getRegEx(startState, (*it), automation.size(), true) + " ]";
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

	if (a.getFinalStates().contains(a.getStartState()) || b.getFinalStates().contains(b.getStartState()))
		result.makeStateFinal(result.getStartState());
	result.alphabet = Union(a.alphabet, b.alphabet);
	return result;
}

FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b)
{
	FiniteStateAutomation result = a;
	CustomSet<int> firstFiniteStateAutomationFinalStates(a.getFinalStates()); // copy c-tor
	result.absorb(b);

	for (auto it = firstFiniteStateAutomationFinalStates.getRaw().begin(); it != firstFiniteStateAutomationFinalStates.getRaw().end(); it++)
		result.copyTransitions(*it, b.getStartState() + a.getStatesCount());

	if (!b.getFinalStates().contains(b.getStartState()))
		result.finalStates = SetDifference(result.finalStates, firstFiniteStateAutomationFinalStates);
	result.alphabet = Union(a.alphabet, b.alphabet);
	return result;
}

FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a)
{
	FiniteStateAutomation result = a;
	result.addState();
	result.changeStartState(result.getStatesCount() - 1);
	result.copyTransitions(result.getStatesCount() - 1, a.getStartState());

	for (auto it = result.getFinalStates().getRaw().begin(); it != result.getFinalStates().getRaw().end(); it++)
		result.copyTransitions(*it, result.getStartState());

	result.makeStateFinal(result.getStartState());

	return result;
}

FiniteStateAutomation Complement(const FiniteStateAutomation& a)
{

	FiniteStateAutomation result = a;
	result.makeDeterministic();
	result.makeTotal();
	CustomSet<int> newFinals;
	for (int i = 0; i < result.automation.size(); ++i)
	{
		if (!result.finalStates.contains(i))
			newFinals.add(i);
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

//TODO : Get eps and other symbols to const

//TODO: Review the regex alg.

std::string FiniteStateAutomation::getRegEx(int start, int end, int bound, bool needEpsilon)
{
	if (bound == 0)
	{
		CustomSet<char> s;

		if (start == end && needEpsilon)
			s.add('$');
		for (int i = 0; i < automation[start].size(); ++i)
		{
			edge currentEdge = automation[start][i];
			if (currentEdge.dest == end)
				s.add(currentEdge.ch);
		}
		std::string str;

		for (auto it = s.getRaw().begin(); it != s.getRaw().end(); it++)
		{
			if (it != s.getRaw().begin())
				str += " + ";
			str += *it;
		}
		return str;
	}
	
	std::string left = getRegEx(start, end, bound - 1, needEpsilon);

	std::string b = getRegEx(start, bound - 1, bound - 1, needEpsilon);
	std::string c = getRegEx(bound - 1, bound - 1, bound - 1, false); // Don't need epsilon (we have * -> it contains it)
	std::string d = getRegEx(bound - 1, end, bound - 1, needEpsilon);

	std::string right;
	if (b != "$")
	{
		if (needBrackets(b))
			right += "(" + b + ")";
		else
			right += b;
	}


	if (c != "$" && c != "")
		right += "(" + c + ")*";

	if (d != "$")
	{
		if (needBrackets(d))
			right += "(" + d + ")";
		else
			right += d;
	}



	if (b == "$" && (c == "$" || c == "") && d == "$")
		right = "$";
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

	for (auto it = alphabet.getRaw().begin(); it != alphabet.getRaw().end(); it++)
		addTransition(ind, ind, *it);
	return ind;
}

AutomationBase* FiniteStateAutomation::clone() const
{
	return new FiniteStateAutomation(*this);
}
