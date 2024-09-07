#include "AFL_Environment.h"

AFL_Environment::AFL_Environment()
{

}

int AFL_Environment::registerAutomation(const std::string& key, AutomationBase* fsa)
{
	auto autPair = automations.find(key);

	automations[key] = fsa;
	return 0;

}
int AFL_Environment::getAutomation(const std::string& key, AutomationBase*& aut)
{
	auto autPair = automations.find(key);
	if (autPair != automations.end())
	{
		aut = automations[key];
		return 0;
	}
	else
		return -1;//does not exists
}

std::string AFL_Environment::toStringAutomations()
{
	std::string result = "Finite-State Automations:\n";

	for (auto it = automations.begin(); it != automations.end(); ++it)
	{
		result += "ID: " + it->first + ", ";
		result += it->second->getString();
	}
	return result;
}

void AFL_Environment::setSecretForAssignments(FiniteStateAutomation* fsa)
{
	fsaSecret = fsa;
}
FiniteStateAutomation* AFL_Environment::GetSecretForAssignments()
{
	return fsaSecret;
}

