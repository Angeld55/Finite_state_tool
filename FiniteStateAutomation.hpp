#ifndef AUTOMAT_HDR
#define AUTOMAT_HDR

#include<iostream>
#include "Queue.hpp"
#include "DynamicArray.hpp"
#include "Set.hpp"
#include "String.hpp"
#include "Dictionary.h"

struct edge {

	int dest;
	char ch;
	edge() {
		dest = -1;
		ch = '/0';
	}

	edge(int dest, char ch) {
		this->dest = dest;
		this->ch = ch;
	}

	void changeDest(int dest) {
		this->dest = dest;
	}
};

class FiniteStateAutomation {

private:
	int startState;
	Set<int> finalStates;
	DynamicArray<DynamicArray<edge>> automata;
	DynamicArray<char>  alphabet;


public:
	FiniteStateAutomation();
	FiniteStateAutomation(int statesCount);
	FiniteStateAutomation(String reg);


	//control
	void AddLetterToAlphabet(char ch);
	int AddState();
	bool AddTransition(int start, int end, char ch); //add edge between two states
	bool ChangeStartState(int state);
	bool MakeStateFinal(int state);


	bool Recognize(String str); //return true if automata recognize the string
	bool IsEmptyLanguage();
	



	//operations
	friend FiniteStateAutomation Union(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a);

	friend FiniteStateAutomation Complement(const FiniteStateAutomation& a);
	friend FiniteStateAutomation InterSection(const FiniteStateAutomation& a, const FiniteStateAutomation& b);
	friend FiniteStateAutomation Reverse(const FiniteStateAutomation& a);

	void Reverse();
	
	bool IsDeterministic();
	
	void MakeComplete();
	void MakeDeterministic();
	void Minimize();
	String GetRegEx(); //kleeny theorem


	int GetStatesCount() const;
	int GetStartState() const;
	Set<int> GetFinalStates() const;

	void Print();


private:

	bool checkIfReachable(int state);
	void removeState(int);

	bool existState(int state); //check if state exists
	void absorb(const FiniteStateAutomation& a);
	void copyTransitions(int x, int y); 
	Set<int> havePathTo(int begin, String str); //return set of states reachable with that word

	//for determinstisation
	Set<int> getTransiotions(int start, char ch);
	Set<int> getTransiotions(Set<int>, char ch);

	//for kleeny theorem
	String getRegEx(int start, int end, int bound);

	//for making complete
	int addErrorState();
};

FiniteStateAutomation BuildFiniteStateAutomation(String reg);
FiniteStateAutomation CreateBaseFiniteStateAutomation(char ch); //automata with two states and one letter


//definitions
FiniteStateAutomation::FiniteStateAutomation() {

	startState = 0;
	AddState();
}

FiniteStateAutomation::FiniteStateAutomation(int statesCount) :automata(statesCount)
{
	startState = 0;
	for (int i = 0; i < statesCount; ++i)
	{
		DynamicArray<edge> t;
		automata.PushBack(t);
	}
}

FiniteStateAutomation::FiniteStateAutomation(String reg)
{
	*this = BuildFiniteStateAutomation(reg);
	bool* temp = new bool[62];//to keep track the symbols that are in the alphabet
	for (int i = 0; i < 62; ++i)
		temp[i] = false;

	for (int i = 0; i < reg.getLenght(); ++i)
	{
		if (reg[i] >= 'a'&&reg[i] <= 'z')
		{
			if (!temp[reg[i] - 97])
			{
				AddLetterToAlphabet(reg[i]);
				temp[reg[i]-97] = true;
			}
		}
		else if (reg[i] >= 'A'&&reg[i] <= 'Z')
		{
			if (!temp[26 + (reg[i] - 65)])
			{
				AddLetterToAlphabet(reg[i]);
				temp[26 + (reg[i] - 65)] = true;
			}
		}
		else if (reg[i] >= '0' && reg[i] <= '9')
		{
			if (!temp[52 + (reg[i] - 48)])
			{
				AddLetterToAlphabet(reg[i]);
				temp[52 + (reg[i] - 48)] = true;
			}
		}
		
	}
	delete[] temp;
}

void FiniteStateAutomation::AddLetterToAlphabet(char ch)
{
	alphabet.PushBack(ch);
}

int FiniteStateAutomation::AddState() {

	DynamicArray<edge> t;
	automata.PushBack(t);
	return automata.getSize() - 1;
}

bool FiniteStateAutomation::ChangeStartState(int state) {

	if (!existState(state))
		return false;
	startState = state;
	return true;
}

bool FiniteStateAutomation::MakeStateFinal(int state) {

	if (!existState(state))
		return false;
	return finalStates.Add(state);
}

int FiniteStateAutomation::GetStatesCount() const {
	return automata.getSize();
}

int FiniteStateAutomation::GetStartState() const {
	return startState;
}

Set<int> FiniteStateAutomation::GetFinalStates() const {
	return finalStates;
}

bool FiniteStateAutomation::AddTransition(int start, int end, char ch) {

	if (!existState(start) && !existState(end))
		return false;
	edge e(end, ch);
	automata[start].PushBack(e);
	return true;
}

bool FiniteStateAutomation::existState(int state) {
	return state < automata.getSize();
}

Set<int> FiniteStateAutomation::havePathTo(int begin, String str) {

	char firstCh = str[0];
	Set<int> result;

	if (str.getLenght() == 0) {
		result.Add(begin);
		return result;
	}

	for (int i = 0; i < automata[begin].getSize(); i++) {

		int otherState = automata[begin][i].dest;
		char symb = automata[begin][i].ch;

		if (symb == firstCh) {
			Set<int> resultStates = havePathTo(otherState, str.SubString(1));
			result = Union(result, resultStates);
		}
	}
	return result;
}

Set<int> FiniteStateAutomation::getTransiotions(int start, char ch)
{
	Set<int> result;
	for (int i = 0; i < automata[start].getSize(); ++i)
	{
		if (automata[start][i].ch == ch)
			result.Add(automata[start][i].dest);
	}
	return result;
}

Set<int> FiniteStateAutomation::getTransiotions(Set<int> s, char ch)
{
	Set<int> result;
	for (int i = 0; i < s.getSize(); ++i)
		result = Union(result, getTransiotions(s.getAt(i), ch));
	return result;
}

bool FiniteStateAutomation::Recognize(String str) {

	Set<int> result = havePathTo(startState, str);
	Set<int> intersection = Intersection(finalStates, result);

	return intersection.getSize();
}

bool FiniteStateAutomation::IsEmptyLanguage() {

	bool* visited = new bool[automata.getSize()];
	for (int i = 0; i < automata.getSize(); i++)
		visited[i] = false;

	Queue<int> gray;
	gray.Push(GetStartState());
	visited[GetStartState()] = true;

	while (!gray.isEmpty()) {

		int currentVertex = gray.Peek();
		gray.Pop();

		if (GetFinalStates().Contains(currentVertex)) {
			delete[] visited;
			return false;
		}

		for (int i = 0; i < automata[currentVertex].getSize(); i++) {

			int neighbor = automata[currentVertex][i].dest;
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				gray.Push(neighbor);
			}
		}
	}
	delete[] visited;
	return true;
}

void FiniteStateAutomation::MakeComplete()
{


	bool* temp = new bool[alphabet.getSize()];
	int errorStateIndex=-1;
	for (int i = 0; i < automata.getSize(); ++i)
	{
		for (int j = 0; j < alphabet.getSize(); ++j) //we mark all letters as not-used
			temp[j] = false;
		for (int j = 0; j < automata[i].getSize(); ++j)
			temp[alphabet.IndexOf(automata[i][j].ch)] = true; //may not be the best implementation. For big set of letter would be slow
		for (int j = 0; j < alphabet.getSize(); ++j) //we check with which of the letters we don't have a transition
		{
			if (!temp[j])
			{
				if (errorStateIndex == -1)
					errorStateIndex = addErrorState();
				AddTransition(i, errorStateIndex, alphabet[j]);
			}
		}
		for (int i = 0; i < alphabet.getSize(); ++i) //we mark all letters as not-used AGAIN (for the next state)
			temp[i] = false;

	}
	delete[] temp;
}

bool FiniteStateAutomation::IsDeterministic() {

	for (int i = 0; i < automata.getSize(); i++) {
		for (int j = 0; j < automata[i].getSize(); j++) {
			for (int k = j + 1; k < automata[i].getSize(); k++) {
				if (automata[i][j].ch == automata[i][k].ch)
					return false;
			}
		}
	}
	return true;
}

void FiniteStateAutomation::absorb(const FiniteStateAutomation& a) {

	int sizeFiniteStateAutomation = automata.getSize();

	for (int k = 0; k < a.automata.getSize(); k++) {
		automata.PushBack(a.automata[k]);

		for (int j = 0; j < a.automata[k].getSize(); j++) 
			automata[k + sizeFiniteStateAutomation][j].changeDest(sizeFiniteStateAutomation + automata[k + sizeFiniteStateAutomation][j].dest);
		
		if (a.finalStates.Contains(k)) 
			finalStates.Add(sizeFiniteStateAutomation + k);
	}
}
			
void FiniteStateAutomation::copyTransitions(int x, int y) {

	int to = automata[y].getSize();

	for (int i = 0; i < to; i++) 
		automata[x].PushBack(automata[y][i]);
}

void FiniteStateAutomation::Print() {

	for (int i = 0; i < automata.getSize(); i++) {

		std::cout << "State: " << i;
		if (i == startState)
			std::cout << "(S)";
		if (finalStates.Contains(i))
			std::cout << "(F)";
		std::cout << " transitions: ";
		for (int j = 0; j < automata[i].getSize(); j++) 
			std::cout << "with " << automata[i][j].ch << " to " << automata[i][j].dest << "   ";
		
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void FiniteStateAutomation::removeState(int state)
{
	automata.RemoveAt(state);
	for (int i = 0; i < automata.getSize(); ++i)
	{
		for (int j = 0; j < automata[i].getSize(); ++j)
		{
			if (automata[i][j].dest>state)
				automata[i][j].dest--;
		}
	}
}

bool t(const Set<int>& l, const Set<int>& r);
void FiniteStateAutomation::MakeDeterministic()
{
	FiniteStateAutomation result;
	result.alphabet = alphabet;
	Queue<Set<int>> newStates;
	Dictionary newStatesIndecies;

	//init start state
	Set<int> newStartState;
	newStartState.Add(startState);
	if (GetFinalStates().Contains(startState))
		result.MakeStateFinal(0);

	newStatesIndecies.put(newStartState, 0);
	newStates.Push(newStartState);

	int statesCount = 1;
	while (!newStates.isEmpty())
	{
		Set<int> currentState = newStates.Pop();
		

		int currentStateIndex = newStatesIndecies.get(currentState);


		for (int i = 0; i < alphabet.getSize(); ++i)
		{
			Set<int> currentStateSet = getTransiotions(currentState, alphabet[i]);
			if (currentStateSet.getSize() == 0)
				continue;
			if (newStatesIndecies.get(currentStateSet) != -1) //if the state exists
 			{
				int destStateIndex = newStatesIndecies.get(currentStateSet);
				result.AddTransition(currentStateIndex, destStateIndex, alphabet[i]);
			}
			else
			{
				result.AddState();
				newStatesIndecies.put(currentStateSet, statesCount++);
				newStates.Push(currentStateSet);
				
				if (Intersection(currentStateSet, finalStates).getSize() != 0)
					result.MakeStateFinal(statesCount - 1);

				result.AddTransition(currentStateIndex, statesCount - 1, alphabet[i]);
			}
		}
	}
	*this = result;

}

void FiniteStateAutomation::Reverse()
{
	FiniteStateAutomation result(automata.getSize());
	result.alphabet = alphabet;

	//reverse all the transition
	for (int i = 0; i < automata.getSize(); ++i)
	{
		for (int j = 0; j < automata[i].getSize(); ++j)
		{
			edge current = automata[i][j];
			result.AddTransition(current.dest, i, current.ch);
		}
	}

	result.MakeStateFinal(startState);

	//new start state
	int newStart = result.AddState();
	result.ChangeStartState(newStart);
	//
	for (int i = 0; i < finalStates.getSize(); ++i)
	{
		int currentFinalState = finalStates.getAt(i);
		for (int j = 0; j < result.automata[currentFinalState].getSize(); ++j)
		{
			edge current = result.automata[currentFinalState][j];
			result.AddTransition(newStart, current.dest, current.ch);
		}
	}
	*this = result;

}

void FiniteStateAutomation::Minimize()
{
	MakeDeterministic();

	Reverse();

	MakeDeterministic();
	
	Reverse();

	MakeDeterministic();
	
}

String FiniteStateAutomation::GetRegEx()
{
	String res;
	for (int i = 0; i < finalStates.getSize(); ++i)
	{
		if (i > 0)
			res += '+';
		res += "[" + getRegEx(startState, finalStates.getAt(i), automata.getSize()) + "]";
	}
	return res;
}

FiniteStateAutomation Union(const FiniteStateAutomation& a, const FiniteStateAutomation& b) {

	FiniteStateAutomation result = a;
	result.absorb(b);
	result.AddState(); //it will be the new start state
	result.ChangeStartState(result.GetStatesCount() - 1); 
	result.copyTransitions(result.GetStatesCount() - 1, a.GetStartState());
	result.copyTransitions(result.GetStatesCount() -1 , a.GetStatesCount() + b.GetStartState());
	
	if (a.GetFinalStates().Contains(a.GetStartState()) || b.GetFinalStates().Contains(b.GetStartState()))
		result.MakeStateFinal(result.GetStartState());

	return result;
}

FiniteStateAutomation Concat(const FiniteStateAutomation& a, const FiniteStateAutomation& b) {

	FiniteStateAutomation result = a;
	Set<int> firstFiniteStateAutomationFinalStats(a.GetFinalStates()); // copy c-tor
	result.absorb(b);
	
	for (int i = 0; i < firstFiniteStateAutomationFinalStats.getSize(); i++) 
		result.copyTransitions(firstFiniteStateAutomationFinalStats.getElement(i), b.GetStartState() + a.GetStatesCount());
	
	if (!b.GetFinalStates().Contains(b.GetStartState())) 
		result.finalStates = SetDifference(result.finalStates, firstFiniteStateAutomationFinalStats);
	
	return result;
}

FiniteStateAutomation KleeneStar(const FiniteStateAutomation& a) {

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
	result.MakeComplete();
	Set<int> newFinals;
	for (int i = 0; i < result.automata.getSize(); ++i)
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

FiniteStateAutomation Reverse(const FiniteStateAutomation& a)
{
	FiniteStateAutomation res(a);
	res.Reverse();
	return res;
}

FiniteStateAutomation BuildFiniteStateAutomation(String reg) {

	FiniteStateAutomation result;

	if (reg.getLenght() == 3)
		result = CreateBaseFiniteStateAutomation(reg[1]); //(a)

	else if (reg[reg.getLenght() - 1] == '*')
		result = KleeneStar(BuildFiniteStateAutomation(reg.SubString(1, reg.getLenght() - 3)));
	else {
		String newReg = reg.SubString(1, reg.getLenght() - 2);  //remove brackets

		int countOpeningBrackets = 0;
		int operationIndex;

		for (int i = 0; i < newReg.getLenght(); i++) {
			if (newReg[i] == '(')
				countOpeningBrackets++;
			else if (newReg[i] == ')')
				countOpeningBrackets--;

			if (countOpeningBrackets == 0) {
				operationIndex = i + 1;
				break;
			}
		}

		if (newReg[operationIndex] == '+')
			result = Union(BuildFiniteStateAutomation(newReg.SubString(0, operationIndex - 1)),
				BuildFiniteStateAutomation(newReg.SubString(operationIndex + 1, newReg.getLenght() - 1)));
		else if (newReg[operationIndex] = '.')
			result = Concat(BuildFiniteStateAutomation(newReg.SubString(0, operationIndex - 1)),
				BuildFiniteStateAutomation(newReg.SubString(operationIndex + 1, newReg.getLenght() - 1)));
	}

	return result;
}

FiniteStateAutomation CreateBaseFiniteStateAutomation(char ch) {

	FiniteStateAutomation a;
	a.AddState();
	a.AddTransition(0, 1, ch);
	a.MakeStateFinal(1);
	return a;
}


String FiniteStateAutomation::getRegEx(int start, int end, int bound)
{
	if (bound == 0)
	{
		Set<char> s;
	
		if (start == end)
			s.Add('e');
		for (int i = 0; i < automata[start].getSize(); ++i)
		{
			edge currentEdge = automata[start][i];
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

	String left = getRegEx(start, end, bound - 1);
	
	String b = getRegEx(start, bound - 1, bound - 1);
	String c = getRegEx(bound - 1, bound - 1, bound - 1);
	String d = getRegEx(bound - 1, end, bound - 1);

	



	String right;

	if (b != "e")
		right += b + "";

	if (c != "e" && c!="")
		right += "(" + c + ")*";
		
	if (d != "e")
		right += "" + d;


	if (b == "å" && (c == "å" || c=="") && d == "e")
		right = "å";

	if (left == "" && right == "")
		return "";
	if (left == "")
		return right;
	if (right == "")
		return left;
	if (left == right)
		return left;

	return left + "+" + "(" + right + ")";

	
}

int FiniteStateAutomation::addErrorState()
{
	int ind = AddState();
	for (int i = 0; i < alphabet.getSize(); ++i)
		AddTransition(ind, ind, alphabet[i]);
	return ind;
}

#endif // !AUTOMAT_HDR
