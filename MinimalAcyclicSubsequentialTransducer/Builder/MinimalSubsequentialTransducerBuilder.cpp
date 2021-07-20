#include "MinimalSubsequentialTransducerBuilder.h"
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;


char getChar(const string& str, size_t ind)
{
	return str[ind - 1];
}

string longestCommonPrefix(const string& s1, const string& s2)
{
	string res;
	size_t i = 0;
	while (i < s1.size() && i < s2.size() && s1[i] == s2[i])
		res += s1[i++];
	return res;
}

size_t longestCommonPrefixLength(const string& s1, const string& s2)
{
	return longestCommonPrefix(s1, s2).size();
}

SubsequentialTransducer createMinimalSubsequentialTransducerBySortedFile(const vector<pair<string, string>>& input, size_t maxWordLength)
{

	SubsequentialTransducer transducer(maxWordLength);

	string currentWord = "";
	string previousWord = "";
	auto begin = clock();
	double totalTime = 0;
	for (int wordInd = 0; wordInd < input.size(); wordInd++)
	{

		string currentWord = input[wordInd].first;
		string currentOutput = input[wordInd].second;

		//nz << currentWord<<"|" << currentOutput << endl;
		//cout << "Input" << currentWord << "!" << currentOutput << endl;
		size_t currentOutputNumber = atoi(currentOutput.c_str());
		size_t prefixLengthPlus1 = longestCommonPrefixLength(currentWord, previousWord) + 1;

		//we minimize the states from the suffix of the previous word 
		for (size_t i = previousWord.size(); i >= prefixLengthPlus1; i--)
			transducer.SET_TRANSITION(i - 1, getChar(previousWord, i), transducer.FindMinimized(i));


		//This loop initializes the tail states for the  current word
		for (size_t i = prefixLengthPlus1; i <= currentWord.size(); i++)
		{
			transducer.CLEAR_STATE(i);
			transducer.SET_TRANSITION(i - 1, getChar(currentWord, i), i);
		}

		if (currentWord != previousWord)
		{
			transducer.SET_FINAL(currentWord.size(), true);
			transducer.SET_STATE_OUTPUT(currentWord.size(), {});
		}
		for (size_t j = 1; j <= prefixLengthPlus1 - 1; j++)
		{

			size_t TEMP = transducer.OUTPUT(j - 1, getChar(currentWord, j));
			size_t commonPrefix = min(TEMP, currentOutputNumber);
			size_t wordSuffix = TEMP - commonPrefix;
			transducer.SET_OUTPUT(j - 1, getChar(currentWord, j), commonPrefix);


			transducer.addNumberToAllTransitionsFromState(j, wordSuffix);

			if (transducer.FINAL(j) && wordSuffix != 0)
				transducer.addNumberToOutputState(j, wordSuffix);

			currentOutputNumber -= commonPrefix;
			transducer.handleStateChange(j);
			
		}
		if (currentWord == previousWord)
		{
			//by the new requirements this case is not possible!
			;
			//auto res = transduser.states[currentWord.size()].outPut.insert(currentOutputNumber);
			//transduser.states[currentWord.size()].hashOutput += (res.second)*(11 * currentOutputNumber);
			//transduser.handleStateChange(currentWord.size());
		}
		else
			transducer.SET_OUTPUT(prefixLengthPlus1 - 1, getChar(currentWord, prefixLengthPlus1), currentOutputNumber);
		
		previousWord = currentWord;

		if (wordInd > 0 && wordInd % 10000 == 0)
		{
			auto end = clock();
			auto elapsed = double(end - begin);
			cout << "Parsed words: " <<  wordInd << endl;
			cout << "Time:         " <<  (elapsed /1000.0) << " seconds" << endl <<endl;
			totalTime += elapsed / 1000.0;
			begin = clock();
		}
	}

	// here we are minimizing the states of the last word
	for (int i = previousWord.size(); i >= 1; i--)
		transducer.SET_TRANSITION(i - 1, getChar(previousWord, i), transducer.FindMinimized(i));

	transducer.initialState = transducer.FindMinimized(0);
	transducer.uselessStates = maxWordLength;

	for (int i = 0; i < transducer.uselessStates; i++)
		transducer.removeFromDict(i);

	auto end = clock();
	auto elapsed = double(end - begin);
	totalTime += elapsed / 1000.0;

	cout << "Total time: " << totalTime << endl;

	return std::move(transducer);
}

void push_outPuts(size_t currentState, int residual, SubsequentialTransducer& st)
{
	st.addNumberToAllTransitionsFromState(currentState, residual);
}

void insertSuffix(size_t currentState, const std::string& word, size_t outPut, SubsequentialTransducer& SubsequentialTransducer, vector<size_t>& statesSequence)
{
	for (size_t i = 1; i <= word.size(); i++)
	{
		char ch = getChar(word, i);
		size_t newCurrent = SubsequentialTransducer.newState();
		SubsequentialTransducer.SET_TRANSITION(currentState, ch, newCurrent);
		if (outPut != 0)
		{
			SubsequentialTransducer.SET_OUTPUT(currentState, ch, outPut);
			outPut = 0;
		}
		statesSequence.push_back(newCurrent);
		currentState = newCurrent;
	}
	SubsequentialTransducer.SET_FINAL(currentState, 1);
}

void removeDuplicates(const string& word, vector<size_t>& statesSequence, SubsequentialTransducer& SubsequentialTransducer)
{
	set<int> d = { 0 };
	if (SubsequentialTransducer.states[statesSequence.back()].isFinal && SubsequentialTransducer.states[statesSequence.back()].outPut == d)
	{
		SubsequentialTransducer.states[statesSequence.back()].outPut.clear();
		SubsequentialTransducer.states[statesSequence.back()].hashOutput = 0;
	}

	for (int i = statesSequence.size() - 2; i >= 0; i--)
	{
		int existingState = -1;
		SubsequentialTransducer.SET_TRANSITION(statesSequence[i], word[i], existingState = SubsequentialTransducer.existsSame(statesSequence[i + 1]));
		
		if (existingState != statesSequence[i + 1])
		{
			SubsequentialTransducer.removeState(statesSequence[i + 1]);
			statesSequence[i + 1] = existingState;
		}
	}
}
void insertWord(const string& word, size_t outPut, SubsequentialTransducer& st)
{
	//cout << "Inserting: " << word << " ---> "<< outPut <<  endl;
	size_t currentState = st.initialState;

	std::vector<size_t> statesSequence = { currentState };
	int i = 1;
	bool foundConfluence = false;

	for (;i <= word.size(); i++)
	{

		char symbol = getChar(word, i);

		auto childIt = st.states[currentState].transitions.find(symbol);

		if (childIt == st.states[currentState].transitions.end())
			break;

		size_t child = (*childIt).second.first;
		int ticket = (*childIt).second.second;

		if (st.states[child].inDegree > 1)
			foundConfluence = true;

		if (foundConfluence)
			st.SET_TRANSITION(currentState, symbol, child = st.CLONE(child));

		statesSequence.push_back(child);

		size_t outPutPrefix = std::min(outPut, st.OUTPUT(currentState, symbol)); // longest prefix
		size_t outPutSuffix = st.OUTPUT(currentState, symbol) - outPutPrefix;
		outPut = outPut - outPutPrefix;

		size_t oldOutPut = st.OUTPUT(currentState, symbol);
		st.SET_OUTPUT(currentState, symbol, outPutPrefix);

		currentState = child;


		if (st.states[currentState].isFinal && outPutSuffix != 0)
			st.addNumberToOutputState(currentState, outPutSuffix);

		push_outPuts(currentState, outPutSuffix, st);
		if (i == word.size())
		{
			st.insertOutPut(currentState, outPut);
			outPut = 0;	
		}

	}

	insertSuffix(currentState, word.substr(i - 1), outPut, st, statesSequence);

	removeDuplicates(word, statesSequence, st);
}

int findMinOutGoing(size_t state, SubsequentialTransducer& st)
{
	int minOutGoing = INT_MAX;
	
	if (st.FINAL(state) && st.states[state].outPut.empty())
		minOutGoing = 0;

	for (auto it = st.states[state].outPut.begin(); it != st.states[state].outPut.end(); it++)
		minOutGoing = std::min(minOutGoing, (*it));

	for (auto it = st.states[state].transitions.begin(); it != st.states[state].transitions.end(); it++)
		minOutGoing = std::min(minOutGoing, (*it).second.second);
	

	return minOutGoing;
}

void reduseAllByOutgoing(size_t state, SubsequentialTransducer& st, int min)
{
	st.substractNumberFromOutputState(state, min);
	st.substractNumberFromAllTransitionsFromState(state, min);
}

void addMinToTransition(SubsequentialTransducer& st, size_t state, char ch, int min)
{
	st.addNumToTransitionTicket(state, ch, min);
}

void removeWord(const string& word, SubsequentialTransducer& st)
{
	size_t currentState = st.initialState;

	std::vector<size_t> statesSequence = { currentState };
	int i = 1;
	bool foundConfluence = false;
	for (; i <= word.size(); i++)
	{

		char symbol = getChar(word, i);

		auto childIt = st.states[currentState].transitions.find(symbol);

		if (childIt == st.states[currentState].transitions.end())
			break;

		size_t child = (*childIt).second.first;
		int ticket = (*childIt).second.second;

		if (st.states[child].inDegree > 1)
			foundConfluence = true;

		if (foundConfluence)
			st.SET_TRANSITION(currentState, symbol, child = st.CLONE(child));

		statesSequence.push_back(child);
		currentState = child;
	}

	st.SET_FINAL(statesSequence.back(), false);

	int t = statesSequence.size() - 1;
	bool lastHasTransitoins = st.states[statesSequence.back()].transitions.size() > 0;

	bool lastState =  true;

	for (; t > 0; t--)
	{
		size_t currentS = statesSequence[t];

		if (st.FINAL(currentS) || st.states[currentS].transitions.size() >= 1) //since we deleted the outgoing transition
		{
			break;
		}
		else
		{
			if (lastState && st.states[currentS].transitions.size() >= 1) //dont delete the final if it has outgoing trans
				continue;
			

			st.removeState(currentS);

			if (t > 0) //remove transition
			{
				size_t prevState = statesSequence[t - 1];
				st.removeTransition(prevState, word[t - 1]);
			}
			statesSequence.pop_back();
			lastState = false;
		}
	}


	//fixing the outputs
	//first state that should not be deleted - is final or has transtiotions


	int k = statesSequence.size() - 1;
	if (k <= 0)
		return;

	size_t validState = statesSequence.back();

	int min = findMinOutGoing(validState, st);
	reduseAllByOutgoing(validState, st, min);

	validState = statesSequence[--k];

	do
	{

		if (min == 0)
			break;

		if (k == 0)
		{
			addMinToTransition(st, statesSequence[0], word[0], min);
			break;
		}
		addMinToTransition(st, statesSequence[k], word[k], min);

		min = findMinOutGoing(validState, st);

		reduseAllByOutgoing(validState, st, min);

		validState = statesSequence[--k];

	} while (true);

	for (int s = statesSequence.size() - 2 ; s >= 0; s--)
	{
		size_t existingState = statesSequence[s + 1];
		st.SET_TRANSITION(statesSequence[s], word[s], existingState = st.existsSame(statesSequence[s + 1]));

		if (existingState != statesSequence[s + 1])
		{
			st.removeState(statesSequence[s + 1]);
			statesSequence[s + 1] = existingState;
		}

	}
}
bool areEquivalentStates(const SubsequentialTransducer& lhs, size_t stateLhs, const SubsequentialTransducer& rhs, size_t stateRhs)
{
	if (lhs.states[stateLhs].outPut != rhs.states[stateRhs].outPut)
		return false;

	if (lhs.states[stateLhs].hashOutput != rhs.states[stateRhs].hashOutput)
		return false;

	if (lhs.states[stateLhs].isFinal ^ rhs.states[stateRhs].isFinal)
		return false;

	if (lhs.states[stateLhs].transitions.size() != rhs.states[stateRhs].transitions.size())
		return false;

	if (lhs.states[stateLhs].inDegree != rhs.states[stateRhs].inDegree)
		return false;

	for (auto lhsIt = lhs.states[stateLhs].transitions.begin(); lhsIt != lhs.states[stateLhs].transitions.end(); lhsIt++)
	{
		auto rhsIt = rhs.states[stateRhs].transitions.find((*lhsIt).first);

		if (rhsIt == rhs.states[stateRhs].transitions.end())
			return false;

		if ((*lhsIt).second.second != (*rhsIt).second.second)
			return false;

		if (!areEquivalentStates(lhs, (*lhsIt).second.first, rhs, (*rhsIt).second.first))
			return false;
	}

	return true;

}
bool areEquivalent(const SubsequentialTransducer& lhs, const SubsequentialTransducer& rhs)
{
	return areEquivalentStates(lhs, lhs.initialState, rhs, rhs.initialState);
}
