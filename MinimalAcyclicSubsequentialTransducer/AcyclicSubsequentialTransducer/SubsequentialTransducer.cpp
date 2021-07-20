#include "SubsequentialTransducer.h"

SubsequentialTransducer::SubsequentialTransducer(size_t statesCount) : states(statesCount) , uselessStates(statesCount)
{
	for (size_t i = 0; i < statesCount; i++)
	{
		states[i].createHashString();
		addToDict(i);
	}
}

size_t SubsequentialTransducer::newState()
{
	states.push_back(state());
	addToDict(states.size() - 1);
	return states.size() - 1;
}

bool SubsequentialTransducer::FINAL(size_t state) const
{
	return states[state].isFinal;
}

void SubsequentialTransducer::SET_FINAL(size_t state, bool isFinal)
{
	states[state].isFinal = isFinal;
	if (!isFinal && !states[state].outPut.empty())
	{
		states[state].outPut.clear();
		states[state].hashOutput = 0;
	}
	handleStateChange(state);
}
size_t SubsequentialTransducer::TRANSITION(size_t state, char ch)  const
{
	auto it = states[state].transitions.find(ch);

	if (it == states[state].transitions.end())
	{
		cout << "problem!" << endl;
		return -1;
	}
	else
		return (*it).second.first;

}

void SubsequentialTransducer::SET_TRANSITION(size_t start, char ch, size_t end)
{
	pair<size_t, size_t> t = { end, 0 };
	auto it = states[start].transitions.find(ch);

	if (it != states[start].transitions.end())
	{

		t.second = (*it).second.second;
		states[start].hashTransiotions -= (7 * ch + 5 * (*it).second.first + 3 * t.second);
		states[(*it).second.first].inDegree--;

	}

	states[start].transitions[ch] = t;
	states[start].hashTransiotions += (7 * ch + 5 * end + 3 * t.second);
	states[end].inDegree++;
	handleStateChange(start);

}

set<int> SubsequentialTransducer::STATE_OUTPUT(size_t state)
{
	return states[state].outPut;
}

void SubsequentialTransducer::SET_STATE_OUTPUT(size_t state, const set<int>& currentSet)
{
	states[state].outPut = currentSet;
	states[state].hashOutput = 0; //used for empty set only

	handleStateChange(state);
}
size_t SubsequentialTransducer::OUTPUT(size_t state, char ch)
{
	const size_t INVALID = 99999;

	auto it = states[state].transitions.find(ch);

	if (it == states[state].transitions.end())
		return INVALID;
	else
		return (*it).second.second;
}
void SubsequentialTransducer::SET_OUTPUT(size_t state, char ch, int str)
{
	auto it = states[state].transitions.find(ch);

	if (it == states[state].transitions.end())
	{
		cout << "Error! Error code 3!" <<endl;
		//throw
	}
	else
	{
		states[state].hashTransiotions -= (7 * ch + 5 * (*it).second.first + 3 * (*it).second.second);
		(*it).second.second = str;
		states[state].hashTransiotions += (7 * ch + 5 * (*it).second.first + 3 * str);
		handleStateChange(state);
	}
}

void SubsequentialTransducer::CLEAR_STATE(size_t state)
{

	states[state].isFinal = false;
	states[state].outPut.clear();
	states[state].transitions.clear();
	///
	states[state].hashOutput = 0;
	states[state].hashTransiotions = 0;
	states[state].inDegree = 0;
	//
	handleStateChange(state);

}

void SubsequentialTransducer::removeFromDict(size_t state)
{
	auto it = statesDict.find(states[state].hashFinal);
	if (it == statesDict.end())
	{
		cout << "Error! Error code: 4";
		//throw
	}
	auto eraseIt = (*it).second.erase(state);

	if (eraseIt == 0)
	{
		cout << "Error! Error code: 1" << endl;
		//throw
	}

	if ((*it).second.empty())
		statesDict.erase(it);
}

void SubsequentialTransducer::addToDict(size_t state)
{
	auto it = statesDict.find(states[state].hashFinal);

	if (it == statesDict.end())
		statesDict[states[state].hashFinal] = { state };
	else
		(*it).second.insert(state);
}

void  SubsequentialTransducer::changeInDegrees(size_t state, bool increment)
{
	for (auto it = states[state].transitions.begin(); it != states[state].transitions.end(); it++)
	{
		if ((*it).second.first >= states.size())
			continue;

		if (increment)
			states[(*it).second.first].inDegree++;
		else
			states[(*it).second.first].inDegree--;
	}
}

size_t SubsequentialTransducer::FindMinimized(size_t stateInd)
{
	auto it = statesDict.find(states[stateInd].hashFinal);
	if (it == statesDict.end())
	{
		state res(states[stateInd]);
		res.inDegree = 0;
		states.push_back(res);
		addToDict(states.size() - 1);
		return states.size() - 1;
	}
	else
	{
		for (auto setIt = (*it).second.begin(); setIt != (*it).second.end(); setIt++)
		{
			if ((*setIt) != stateInd && (*setIt) >= uselessStates && states[*setIt] == states[stateInd])
			{
				changeInDegrees(stateInd, false);
				return *setIt;
			}

		}

		state res(states[stateInd]);
		res.inDegree = 0;
		states.push_back(res);
		addToDict(states.size() - 1);
		return states.size() - 1;
	}
}

size_t SubsequentialTransducer::existsSame(size_t s)
{
	auto it = statesDict.find(states[s].hashFinal);
	if (it == statesDict.end())
	{
		cout << "Error! Error code: 2!" << endl;
	}
	else
	{
		for (auto setIt = (*it).second.begin(); setIt != (*it).second.end(); setIt++)
		{
			if ((*setIt) >= uselessStates && (*setIt) != s && states[*setIt] == states[s])
				return *setIt;
		}	
		return s;
	}
}

void SubsequentialTransducer::handleStateChange(size_t state)
{
	removeFromDict(state);
	states[state].createHashString();
	addToDict(state);
}
void SubsequentialTransducer::smallPrint()
{
	cout << "-----------------------------------------------------" << endl;
	cout << "States count: " << states.size() - uselessStates - otherUselessStates.size() << endl;

	size_t finalStates = 0;
	size_t transiotionsCount = 0;
	for (int i = 0; i < states.size(); i++)
	{
		if (i < uselessStates )
			continue;

		auto it = otherUselessStates.find(i);

		if (it != otherUselessStates.end())
			continue;

		transiotionsCount += states[i].transitions.size();

		if (states[i].isFinal)
		{
			finalStates++;
		}
	}
	cout << "Transitions: " << transiotionsCount << endl;
	cout << "Final states: " << finalStates << endl;
	cout << "-----------------------------------------------------" << endl;
}
void SubsequentialTransducer::PRINTTRANSDUSER(bool onlyStatesCount)
{
	cout << "States count: " << states.size() - uselessStates - otherUselessStates.size() << endl;
	if (onlyStatesCount)
		return;
	for (int i = 0; i < states.size(); i++)
	{
		if (i < uselessStates)
			 continue;
		auto it = otherUselessStates.find(i);

		if (it != otherUselessStates.end())
			continue;
		
		if (initialState == i)
			cout << "(S)";
		else
			cout << "   ";

		cout << "State: " << i << " " << states[i].print() << ", Indegree: "<<states[i].inDegree << endl;
	}
	cout << endl << endl << endl;
}

size_t SubsequentialTransducer::CLONE(size_t stateInd)
{
	states.push_back(state(states[stateInd]));
	states[states.size() - 1].inDegree = 0;
	addToDict(states.size() - 1);
	for (auto it = states[states.size() - 1].transitions.begin(); it != states[states.size() - 1].transitions.end(); it++)
		states[(*it).second.first].inDegree++;
	return states.size() - 1;
}

vector<size_t> SubsequentialTransducer::tranfsorm(const string& str)
{
	size_t res = 0;
	size_t currentState = initialState;

	for (int i = 0; i < str.size(); i++)
	{
		auto trans = states[currentState].transitions.find(str[i]);

		if (trans == states[currentState].transitions.end())
			return{};

		currentState = (*trans).second.first;
		res += (*trans).second.second;
	}
	if (states[currentState].outPut.empty())
		return{ res };

	vector<size_t> resultVector;
	for (auto it = states[currentState].outPut.begin(); it != states[currentState].outPut.end(); it++)
		resultVector.push_back(res + (*it));

	return resultVector;
}

void SubsequentialTransducer::removeState(size_t state)
{
	changeInDegrees(state, false);
	removeFromDict(state);
	if (state == states.size() - 1)
		states.pop_back();
	else
		otherUselessStates.insert(state);
}

bool SubsequentialTransducer::removeTransition(size_t state, char ch)
{
	auto trans = states[state].transitions.find(ch);

	states[state].hashTransiotions -= (7 * (*trans).first + 5 * (*trans).second.first + 3 * (*trans).second.second);
	handleStateChange(state);

	states[state].transitions.erase(ch);

	return true;
}


bool SubsequentialTransducer::addNumToTransitionTicket(size_t state, char ch, int n)
{
	auto trans = states[state].transitions.find(ch);
	states[state].hashTransiotions -= (7 * (*trans).first + 5 * (*trans).second.first + 3 * (*trans).second.second);
	(*trans).second.second += n;
	states[state].hashTransiotions += (7 * (*trans).first + 5 * (*trans).second.first + 3 * (*trans).second.second);
	handleStateChange(state);
	return true;
}

bool SubsequentialTransducer::substractNumberFromAllTransitionsFromState(size_t state, int substract)
{
	size_t hashTransitions = 0;
	for (auto it = states[state].transitions.begin(); it != states[state].transitions.end(); it++)
	{
		(*it).second.second -= substract;
		hashTransitions += (7 * (*it).first + 5 * (*it).second.first + 3 * (*it).second.second);
	}
	states[state].hashTransiotions = hashTransitions;
	handleStateChange(state);
	return true;
}

bool SubsequentialTransducer::addNumberToAllTransitionsFromState(size_t state, int add)
{
	size_t transHash = 0;
	for (auto it = states[state].transitions.begin(); it != states[state].transitions.end(); it++)
	{
		(*it).second.second = add + (*it).second.second;
		transHash += (7 * (*it).first + 5 * (*it).second.first + 3 * ((*it).second.second));
	}

	states[state].hashTransiotions = transHash;
	handleStateChange(state);
	return true;

}
bool SubsequentialTransducer::substractNumberFromOutputState(size_t state, int substract)
{
	set<int> newOutPutSet;
	size_t outPutHash = 0;
	for (auto it = states[state].outPut.begin(); it != states[state].outPut.end(); it++)
	{
		int newOutPut = (*it) - substract;
		newOutPutSet.insert(newOutPut);
		outPutHash += (11 * newOutPut);
	}

	if (newOutPutSet.find(0) != newOutPutSet.end()) //{0}
		newOutPutSet.clear();

	states[state].outPut = newOutPutSet;
	states[state].hashOutput = outPutHash;

	return true;

}

bool SubsequentialTransducer::addNumberToOutputState(size_t state, int add)
{
	if (states[state].outPut.empty())
	{

		auto it = states[state].outPut.insert(add);
		if (it.second)
		{
			states[state].hashOutput += 11 * add;
			handleStateChange(state);
		}
	}
	else
	{
		set<int> newOutPut;
		size_t outPutHash = 0;
		for (auto it2 = states[state].outPut.begin(); it2 != states[state].outPut.end(); it2++)
		{
			newOutPut.insert((*it2) + add);
			outPutHash += 11 * ((*it2) + add);
		}
		states[state].outPut = newOutPut;
		states[state].hashOutput = outPutHash;
		handleStateChange(state);
	}
	return true;
}
bool SubsequentialTransducer::insertOutPut(size_t state, int outPut)
{
	auto itInsert = states[state].outPut.insert(outPut);

	if (itInsert.second)
	{
		states[state].hashOutput += 11 * outPut;
		handleStateChange(state);
	}
	return true;

}