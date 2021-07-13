#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct state
{
	set<int> outPut; // << 11 * elements>
	bool isFinal = 0;
	map<char, pair<size_t, int>> transitions; // <<what>, [<dest> <transduse>]>
	//<<what>, [<dest> <transduse>]>  // <7 * what + 5 * dest + 3 * transduse>

	string hashString;

	int inDegree = 0;

	size_t hashOutput = 0;
	size_t hashTransiotions = 0;
	size_t hashFinal = 0;

	void createHashString()
	{
		hashFinal = 2 * (hashOutput + hashTransiotions) + isFinal;
	}

	string print()
	{
		string res = "Final: " + to_string(isFinal) + " ";

		set<int>::iterator it = outPut.begin();
		if (it != outPut.end())
		{
			res += "Output: ";
		}
		while (it != outPut.end())
		{
			res += to_string((*it)) + " ";
			it++;
		}
		res += "   ";

		for (auto it = transitions.begin(); it != transitions.end(); it++)
		res += "[" + std::string(1,it->first) + ", (" + to_string(it->second.first) + ", " + to_string(it->second.second) + ")] "; 

		return std::move(res);
	}
	bool operator==(const state& other)
	{
		return isFinal == other.isFinal && outPut == other.outPut && transitions == other.transitions;
	}
};

struct SubsequentialTransducer
{
	SubsequentialTransducer(size_t statesCount);

	vector<state> states;
	unordered_map<size_t, set<size_t>> statesDict;
	size_t initialState;
	size_t uselessStates = 0;
	unordered_set<size_t> otherUselessStates;

	void removeFromDict(size_t state);
	void addToDict(size_t state);
	void handleStateChange(size_t state);

	size_t newState();
	bool FINAL(size_t state) const;
	void SET_FINAL(size_t state, bool isFinal);
	size_t TRANSITION(size_t state, char ch) const;
	void SET_TRANSITION(size_t start, char ch, size_t end);

	set<int> STATE_OUTPUT(size_t state);
	void SET_STATE_OUTPUT(size_t state, const set<int>& currentSet);

	size_t OUTPUT(size_t state, char ch);
	void SET_OUTPUT(size_t state, char ch, int str);

	size_t FindMinimized(size_t s);
	size_t existsSame(size_t s);
	
	void CLEAR_STATE(size_t state);
	
	void PRINTTRANSDUSER(bool onlyStatesCount = false);
	void smallPrint();

	vector<size_t> tranfsorm(const string& str);

	size_t CLONE(size_t state);

	void changeInDegrees(size_t state, bool increment);

	void removeState(size_t state);

	bool removeTransition(size_t state, char ch);
	bool addNumToTransitionTicket(size_t state, char ch, int n);

	bool addNumberToAllTransitionsFromState(size_t state, int add);
	bool substractNumberFromAllTransitionsFromState(size_t state, int substract);
	bool substractNumberFromOutputState(size_t state, int substract);
	bool addNumberToOutputState(size_t state, int add);
	bool insertOutPut(size_t state, int outPut);

	size_t collisions = 0;
};

