#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "../Non-deterministic_pushdown_automaton/NPDA.h"
#include "../Finate_state_automation/FiniteStateAutomation.h"

class AFL_Environment
{
	std::map<std::string, FiniteStateAutomation> fsas;
	std::map<std::string, NPDA> npdas;

	FiniteStateAutomation fsaSecret; //temp place for assignments;

public:
	int registerFSA(const std::string& key, const FiniteStateAutomation& fsa);
	int registerNPDA(const std::string& key, const NPDA& fsa);
	int getNPDA(const std::string& key, NPDA*& npda);
	int getFSA(const std::string& key, FiniteStateAutomation*& fsa);
	void setSecretForAssignments(FiniteStateAutomation& fsa);
	FiniteStateAutomation& GetSecretForAssignments();
	std::string toStringFSA();
	std::string toStringNPDA();
};