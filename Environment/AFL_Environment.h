#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../Non-deterministic_pushdown_automaton/NPDA.h"
#include "../Finate_state_automation/FiniteStateAutomation.h"
#include "../GraphVisualizationTool/GraphVisualiser.h"

class AFL_Environment
{
	std::map<std::string, AutomationBase*> automations;
	

	FiniteStateAutomation* fsaSecret; //temp place for assignments;

public:
	GraphVisualiser vis;

	AFL_Environment();
	int registerAutomation(const std::string& key, AutomationBase* npda);

	int getAutomation(const std::string& key, AutomationBase*& aut);


	void setSecretForAssignments(FiniteStateAutomation* fsa);
	FiniteStateAutomation* GetSecretForAssignments();
	std::string toStringAutomations();

};