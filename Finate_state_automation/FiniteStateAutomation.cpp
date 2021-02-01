#include "FiniteStateAutomation.h"
#include <stack>
#include <string>

//definitions
FiniteStateAutomation::FiniteStateAutomation()
{
	startState = 0;
	addState();
}

FiniteStateAutomation::FiniteStateAutomation(int statesCount) :automation(statesCount)
{
	startState = 0;
	for (int i = 0; i < statesCount; ++i)
	{
		DynamicArray<edge> t;
		automation.PushBack(t);
	}
}

FiniteStateAutomation::FiniteStateAutomation(const CustomString& reg)
{
	*this = BuildFiniteStateAutomation(reg);
}

void FiniteStateAutomation::addLetterToAlphabet(char ch)
{
	alphabet.Add(ch);
}

int FiniteStateAutomation::addState()
{
	DynamicArray<edge> t;
	automation.PushBack(t);
	return automation.getSize() - 1;
}

bool FiniteStateAutomation::changeStartState(int state)
{
	if (!existState(state))
		return false;
	startState = state;
	return true;
}

bool FiniteStateAutomation::makeStateFinal(int state)
{
	if (!existState(state))
		return false;
	return finalStates.Add(state);
}

int FiniteStateAutomation::getStatesCount() const
{
	return automation.getSize();
}

int FiniteStateAutomation::getStartState() const
{
	return startState;
}

Set<int> FiniteStateAutomation::getFinalStates() const
{
	return finalStates;
}

bool FiniteStateAutomation::addTransition(int start, int end, char ch)
{
	if (!existState(start) || !existState(end))
		return false;
	edge e(end, ch);
	automation[start].PushBack(e);
	alphabet.Add(ch);
	return true;
}

bool FiniteStateAutomation::existState(int state)
{
	return state < automation.getSize();
}

Set<int> FiniteStateAutomation::havePathTo(int begin,const CustomString& str)
{
	char firstCh = str[0];
	Set<int> result;

	if (str.getLenght() == 0)
	{
		result.Add(begin);
		return result;
	}

	for (int i = 0; i < automation[begin].getSize(); i++)
	{
		int otherState = automation[begin][i].dest;
		char symb = automation[begin][i].ch;

		if (symb == firstCh)
		{
			Set<int> resultStates = havePathTo(otherState, str.SubString(1));
			result = Union(result, resultStates);
		}
	}
	return result;
}

void FiniteStateAutomation::CheckIfOneStated()
{
	FiniteStateAutomation temp = *this;
	temp.makeTotal();

	if (temp.getFinalStates().getSize() == temp.automation.getSize() || temp.getFinalStates().getSize() == 0)
	{
		FiniteStateAutomation a;
		if (getFinalStates().getSize() != 0)
			a.makeStateFinal(0);
		for (int i = 0; i < automation[startState].getSize(); i++)
			a.addTransition(0, 0, automation[startState][i].ch);
		*this = a;
	}
}

void FiniteStateAutomation::removeNotReachable(int from)
{
	DynamicArray<int> notR = getNotReachableStates(from);
	notR.Sort();
	for (int i = notR.getSize() - 1; i >= 0; --i)
		removeState(notR[i]);
}

DynamicArray<int> FiniteStateAutomation::getNotReachableStates(int from)
{
	bool* temp = new bool[automation.getSize()];
	for (int i = 0; i < automation.getSize(); ++i)
		temp[i] = false;

	DFS(from, temp);

	DynamicArray<int> unvisited;
	for (int i = 0; i < automation.getSize(); ++i)
	{
		if (!temp[i])
			unvisited.PushBack(i);

	}
	delete[] temp;
	return unvisited;

}

void FiniteStateAutomation::DFS(int state, bool* visited)
{

	visited[state] = true;
	for (int i = 0; i < automation[state].getSize(); ++i)
	{
		if (!visited[automation[state][i].dest])
			DFS(automation[state][i].dest, visited);
	}
}

FiniteStateAutomation FiniteStateAutomation::reverseTransitions()
{
	FiniteStateAutomation result(automation.getSize());
	result.alphabet = alphabet;

	//reverse all the transition
	for (int i = 0; i < automation.getSize(); ++i)
	{
		for (int j = 0; j < automation[i].getSize(); ++j)
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
	for (int i = 0; i < automation[start].getSize(); ++i)
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
	automation.RemoveAt(state);
	for (int i = 0; i < automation.getSize(); ++i)
	{

		for (int j = 0; j < automation[i].getSize(); ++j)
		{
			if (automation[i][j].dest>state)
				automation[i][j].dest--;
			else if (automation[i][j].dest == state)//may not be the case if it's unreachable
				automation[i].RemoveAt(j--);
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

bool FiniteStateAutomation::accepts(const CustomString& str)
{
	Set<int> result = havePathTo(startState, str);
	Set<int> intersection = Intersection(finalStates, result);

	return intersection.getSize();
}

bool FiniteStateAutomation::isEmptyLanguage()
{
	bool* visited = new bool[automation.getSize()];
	for (int i = 0; i < automation.getSize(); i++)
		visited[i] = false;

	Queue<int> gray;
	gray.Push(getStartState());
	visited[getStartState()] = true;

	while (!gray.isEmpty()) {

		int currentVertex = gray.Peek();
		gray.Pop();

		if (getFinalStates().Contains(currentVertex))
		{
			delete[] visited;
			return false;
		}

		for (int i = 0; i < automation[currentVertex].getSize(); i++)
		{
			int neighbor = automation[currentVertex][i].dest;
			if (!visited[neighbor])
			{
				visited[neighbor] = true;
				gray.Push(neighbor);
			}
		}
	}
	delete[] visited;
	return true;
}
bool  FiniteStateAutomation::isTotal()
{
	bool* temp = new bool[alphabet.getSize()];
	int errorStateIndex = -1;
	for (int i = 0; i < automation.getSize(); ++i)
	{
		for (int j = 0; j < alphabet.getSize(); ++j) 
			temp[j] = false;
		for (int j = 0; j < automation[i].getSize(); ++j)
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
	for (int i = 0; i < automation.getSize(); ++i)
	{
		for (int j = 0; j < alphabet.getSize(); ++j) //we mark all letters as not-used
			temp[j] = false;
		for (int j = 0; j < automation[i].getSize(); ++j)
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

bool FiniteStateAutomation::isDeterministic()
{
	for (int i = 0; i < automation.getSize(); i++)
	{
		for (int j = 0; j < automation[i].getSize(); j++)
		{
			for (int k = j + 1; k < automation[i].getSize(); k++)
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
	int sizeFiniteStateAutomation = automation.getSize();

	for (int k = 0; k < a.automation.getSize(); k++)
	{
		automation.PushBack(a.automation[k]);

		for (int j = 0; j < a.automation[k].getSize(); j++)
			automation[k + sizeFiniteStateAutomation][j].changeDest(sizeFiniteStateAutomation + automation[k + sizeFiniteStateAutomation][j].dest);

		if (a.finalStates.Contains(k))
			finalStates.Add(sizeFiniteStateAutomation + k);
	}
}

void FiniteStateAutomation::copyTransitions(int x, int y)
{
	int to = automation[y].getSize();

	for (int i = 0; i < to; i++)
		automation[x].PushBack(automation[y][i]);
}

std::string FiniteStateAutomation::getString()
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
	res += ("}, States count: " + std::to_string(automation.getSize()) + ".\n");
	return res;
}
void FiniteStateAutomation::print()
{
	for (int i = 0; i < automation.getSize(); i++)
	{
		std::cout << "State: " << i;
		if (i == startState)
			std::cout << "(S)";
		if (finalStates.Contains(i))
			std::cout << "(F)";
		std::cout << " transitions: ";
		for (int j = 0; j < automation[i].getSize(); j++)
			std::cout << "with " << automation[i][j].ch << " to " << automation[i][j].dest << "   ";

		std::cout << std::endl;
	}
	std::cout << std::endl;
}
std::string FiniteStateAutomation::getFullString()
{
	std::string res;
	for (int i = 0; i < automation.getSize(); i++)
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
		for (int j = 0; j < automation[i].getSize(); j++)
			res =  res + "with " + automation[i][j].ch + " to " + std::to_string(automation[i][j].dest) + "   ";
		if (i != automation.getSize()-1)
			res += '\n';
	}

	return res;
}
bool t(const Set<int>& l, const Set<int>& r);
void FiniteStateAutomation::makeDeterministic()
{
	FiniteStateAutomation result;
	result.alphabet = alphabet;
	result.makingMinimal = makingMinimal;
	Queue<Set<int>> newStates;
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
	newStates.Push(newStartState);

	int statesCount = 1;
	while (!newStates.isEmpty())
	{
		Set<int> currentState = newStates.Pop();


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
				newStates.Push(currentStateSet);

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
		for (int j = 0; j < result.automation[currentFinalState].getSize(); ++j)
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

CustomString FiniteStateAutomation::getRegEx()
{
	CustomString res = "";
	for (int i = 0; i < finalStates.getSize(); ++i)
	{
		if (i > 0)
			res += '+';
		res += "[ " + getRegEx(startState, finalStates.getAt(i), automation.getSize(), true) + " ]";
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
	for (int i = 0; i < result.automation.getSize(); ++i)
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
int getClosingBracketIndex(const CustomString& reg)
{
	int count = 1;
	for (int i = 1; i < reg.getLenght(); i++)
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
CustomString convertRegexToRPN(const CustomString& regex)
{
	CustomString result;
	std::stack<char> operatorStack;

	for (int i = 0; i < regex.getLenght(); i++)
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
		if (i != regex.getLenght() - 1)
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

FiniteStateAutomation BuildFiniteStateAutomation(const CustomString& reg)
{
	CustomString regexRPN = convertRegexToRPN(reg);
	stack<FiniteStateAutomation> automationStack;

	for (int i = 0; i < regexRPN.getLenght(); i++)
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
	a.addTransition(0, 1, ch);
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
bool needBrackets(CustomString regex)
{
	if (regex.getLenght() == 0)
		return false;
	if (regex[0] == '(' && regex[regex.getLenght() - 1] == ')')
		return false;

	for (int i = 0; i < regex.getLenght(); ++i)
	{
		if (regex[i] == '+')
			return true;
	}
	return regex[regex.getLenght() - 1] == '*';
}

CustomString FiniteStateAutomation::getRegEx(int start, int end, int bound, bool needEpsilon)
{
	if (bound == 0)
	{
		Set<char> s;

		if (start == end && needEpsilon)
			s.Add('e');
		for (int i = 0; i < automation[start].getSize(); ++i)
		{
			edge currentEdge = automation[start][i];
			if (currentEdge.dest == end)
				s.Add(currentEdge.ch);
		}
		CustomString str;
		for (int i = 0; i < s.getSize(); ++i)
		{
			if (i != 0)
				str += " + ";
			str += s.getAt(i);
		}
		return str;
	}

	CustomString left = getRegEx(start, end, bound - 1, needEpsilon);

	CustomString b = getRegEx(start, bound - 1, bound - 1, needEpsilon);
	CustomString c = getRegEx(bound - 1, bound - 1, bound - 1, false); // Don't need epsilon (we have * -> it contains it)
	CustomString d = getRegEx(bound - 1, end, bound - 1, needEpsilon);

	CustomString right;
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

	CustomString regex = left;
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



