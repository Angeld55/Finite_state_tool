#include "FiniteStateAutomation.h"
#include <stack>

//definitions
FiniteStateAutomation::FiniteStateAutomation()
{
	startState = 0;
	AddState();
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

FiniteStateAutomation::FiniteStateAutomation(const String& reg)
{
	*this = BuildFiniteStateAutomation(reg);
}

void FiniteStateAutomation::AddLetterToAlphabet(char ch)
{
	alphabet.Add(ch);
}

int FiniteStateAutomation::AddState()
{
	DynamicArray<edge> t;
	automation.PushBack(t);
	return automation.getSize() - 1;
}

bool FiniteStateAutomation::ChangeStartState(int state)
{
	if (!existState(state))
		return false;
	startState = state;
	return true;
}

bool FiniteStateAutomation::MakeStateFinal(int state)
{
	if (!existState(state))
		return false;
	return finalStates.Add(state);
}

int FiniteStateAutomation::GetStatesCount() const
{
	return automation.getSize();
}

int FiniteStateAutomation::GetStartState() const
{
	return startState;
}

Set<int> FiniteStateAutomation::GetFinalStates() const
{
	return finalStates;
}

bool FiniteStateAutomation::AddTransition(int start, int end, char ch)
{
	if (!existState(start) && !existState(end))
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

Set<int> FiniteStateAutomation::havePathTo(int begin, String str)
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
	temp.MakeTotal();

	if (temp.GetFinalStates().getSize() == temp.automation.getSize() || temp.GetFinalStates().getSize() == 0)
	{
		FiniteStateAutomation a;
		if (GetFinalStates().getSize() != 0)
			a.MakeStateFinal(0);
		for (int i = 0; i < automation[startState].getSize(); i++)
			a.AddTransition(0, 0, automation[startState][i].ch);
		*this = a;
	}
}

void FiniteStateAutomation::removeNotReachable(int from)
{
	DynamicArray<int> notR = getNotReachableStates(from);
	notR.Sort();
	for (int i = notR.getSize() - 1; i >= 0; --i)
		RemoveState(notR[i]);
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

FiniteStateAutomation FiniteStateAutomation::ReverseTransitions()
{
	FiniteStateAutomation result(automation.getSize());
	result.alphabet = alphabet;

	//reverse all the transition
	for (int i = 0; i < automation.getSize(); ++i)
	{
		for (int j = 0; j < automation[i].getSize(); ++j)
		{
			edge current = automation[i][j];
			result.AddTransition(current.dest, i, current.ch);
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

Set<int> FiniteStateAutomation::getTransitions(Set<int> s, char ch)
{
	Set<int> result;
	for (int i = 0; i < s.getSize(); ++i)
		result = Union(result, getTransitions(s.getAt(i), ch));
	return result;
}

void FiniteStateAutomation::RemoveState(int state)
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

void FiniteStateAutomation::RemoveNotReachable()
{
	removeNotReachable(startState);
}

bool FiniteStateAutomation::Accepts(String str)
{
	Set<int> result = havePathTo(startState, str);
	Set<int> intersection = Intersection(finalStates, result);

	return intersection.getSize();
}

bool FiniteStateAutomation::IsEmptyLanguage()
{
	bool* visited = new bool[automation.getSize()];
	for (int i = 0; i < automation.getSize(); i++)
		visited[i] = false;

	Queue<int> gray;
	gray.Push(GetStartState());
	visited[GetStartState()] = true;

	while (!gray.isEmpty()) {

		int currentVertex = gray.Peek();
		gray.Pop();

		if (GetFinalStates().Contains(currentVertex))
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

void FiniteStateAutomation::MakeTotal()
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
				AddTransition(i, errorStateIndex, alphabet.getAt(j));
			}
		}
		for (int i = 0; i < alphabet.getSize(); ++i) //we mark all letters as not-used AGAIN (for the next state)
			temp[i] = false;

	}
	delete[] temp;
}

bool FiniteStateAutomation::IsDeterministic()
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

void FiniteStateAutomation::Print()
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

bool t(const Set<int>& l, const Set<int>& r);
void FiniteStateAutomation::MakeDeterministic()
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
		if (GetFinalStates().Contains(startState))
			result.MakeStateFinal(0);
	}
	else
	{
		newStartState = starts;
		if (shouldStartBeFinal)
			result.MakeStateFinal(0);
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
				result.AddTransition(currentStateIndex, destStateIndex, alphabet.getAt(i));
			}
			else
			{
				result.AddState();
				newStatesIndecies.put(currentStateSet, statesCount++);
				newStates.Push(currentStateSet);

				if (Intersection(currentStateSet, finalStates).getSize() != 0)
					result.MakeStateFinal(statesCount - 1);

				result.AddTransition(currentStateIndex, statesCount - 1, alphabet.getAt(i));
			}
		}
	}
	result.MakeTotal();
	*this = result;
	//Print();
}

void FiniteStateAutomation::Reverse()
{
	FiniteStateAutomation result = ReverseTransitions();
	result.MakeStateFinal(startState);
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
	int newStart = result.AddState();
	result.ChangeStartState(newStart);

	//the new start state is final if the old start state was final.
	if (GetFinalStates().Contains(startState))
		result.MakeStateFinal(newStart);

	//add the transiotions to the new start state.
	for (int i = 0; i < finalStates.getSize(); ++i)
	{
		int currentFinalState = finalStates.getAt(i);
		for (int j = 0; j < result.automation[currentFinalState].getSize(); ++j)
		{
			edge current = result.automation[currentFinalState][j];
			result.AddTransition(newStart, current.dest, current.ch);
		}
	}
	*this = result;

}

void FiniteStateAutomation::Minimize()
{

	MakeDeterministic();
	makingMinimal = true;
	Reverse();
	MakeDeterministic();
	Reverse();
	MakeDeterministic();
	makingMinimal = false;

}

String FiniteStateAutomation::GetRegEx()
{
	String res = "";
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
	result.AddState(); //it will be the new start state
	result.ChangeStartState(result.GetStatesCount() - 1);
	result.copyTransitions(result.GetStatesCount() - 1, a.GetStartState());
	result.copyTransitions(result.GetStatesCount() - 1, a.GetStatesCount() + b.GetStartState());

	if (a.GetFinalStates().Contains(a.GetStartState()) || b.GetFinalStates().Contains(b.GetStartState()))
		result.MakeStateFinal(result.GetStartState());
	result.alphabet = Union(a.alphabet, b.alphabet);
	return result;
}

FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b)
{
	FiniteStateAutomation result = a;
	Set<int> firstFiniteStateAutomationFinalStats(a.GetFinalStates()); // copy c-tor
	result.absorb(b);

	for (int i = 0; i < firstFiniteStateAutomationFinalStats.getSize(); i++)
		result.copyTransitions(firstFiniteStateAutomationFinalStats.getElement(i), b.GetStartState() + a.GetStatesCount());

	if (!b.GetFinalStates().Contains(b.GetStartState()))
		result.finalStates = SetDifference(result.finalStates, firstFiniteStateAutomationFinalStats);
	result.alphabet = Union(a.alphabet, b.alphabet);
	return result;
}

FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a)
{
	FiniteStateAutomation result = a;
	result.AddState();
	result.ChangeStartState(result.GetStatesCount() - 1);
	result.copyTransitions(result.GetStatesCount() - 1, a.GetStartState());

	for (int i = 0; i < result.GetFinalStates().getSize(); i++)
		result.copyTransitions(result.GetFinalStates().getElement(i), result.GetStartState());
	result.MakeStateFinal(result.GetStartState());

	return result;
}

FiniteStateAutomation Complement(const FiniteStateAutomation& a)
{

	FiniteStateAutomation result = a;
	result.MakeDeterministic();
	result.MakeTotal();
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
int getClosingBracketIndex(const String& reg)
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
String convertRegexToRPN(const String& regex)
{
	String result;
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
			if ((isSymbol(l) && isSymbol(r) || r == '(') || (l == ')' && (isSymbol(r) || r == '(')) ||( l == '*' && (r == '(' || isSymbol(r))))
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

FiniteStateAutomation BuildFiniteStateAutomation(String reg)
{
	String regexRPN = convertRegexToRPN(reg);
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
		a.MakeStateFinal(0);
		return a;
	}
	a.AddLetterToAlphabet(ch);
	a.AddState();
	a.AddTransition(0, 1, ch);
	a.MakeStateFinal(1);
	return a;
}

inline FiniteStateAutomation Reverse(const FiniteStateAutomation& a)
{
	FiniteStateAutomation res(a);
	res.Reverse();
	return res;
}

//if a regex need brackets for better understanding in Auomation to regex conversation.
bool needBrackets(String regex)
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

String FiniteStateAutomation::getRegEx(int start, int end, int bound, bool needEpsilon)
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
		String str;
		for (int i = 0; i < s.getSize(); ++i)
		{
			if (i != 0)
				str += " + ";
			str += s.getAt(i);
		}
		return str;
	}

	String left = getRegEx(start, end, bound - 1, needEpsilon);

	String b = getRegEx(start, bound - 1, bound - 1, needEpsilon);
	String c = getRegEx(bound - 1, bound - 1, bound - 1, false); // Don't need epsilon (we have * -> it contains it)
	String d = getRegEx(bound - 1, end, bound - 1, needEpsilon);

	String right;
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

	String regex = left;
	if (needBrackets(right))
		regex = regex + " + " + "(" + right + ")";
	else
		regex += " + " + right;
	return regex;


}

int FiniteStateAutomation::addErrorState()
{
	int ind = AddState();
	for (int i = 0; i < alphabet.getSize(); ++i)
		AddTransition(ind, ind, alphabet.getAt(i));
	return ind;
}



