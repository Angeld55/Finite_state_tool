#include "Environment.h"

int Environment::registerFSA(const std::string& key, const FiniteStateAutomation& fsa)
{
	auto autPair= fsas.find(key);
	fsas[key] = fsa;
	return 0;

}
int Environment::registerNPDA(const std::string& key, const NPDA& fsa)
{
	auto autPair = npdas.find(key);

	npdas[key] = fsa;
	return 0;

}
int Environment::getNPDA(const std::string& key, NPDA& npda)
{
	auto autPair = npdas.find(key);
	if (autPair != npdas.end())
	{
		npda = npdas[key];
		return 0;
	}
	else
		return -1;//does not exists
}
int Environment::getFSA(const std::string& key, FiniteStateAutomation& fsa)
{
	auto autPair = fsas.find(key);
	if (autPair != fsas.end())
	{
		fsa = fsas[key];
		return 0;
	}
	else
		return -1;//does not exists
}
std::string Environment::print()
{
	std::string result = "Finite-State Automations:\n";

	for (auto it = fsas.begin(); it != fsas.end(); ++it)
	{
		result += "ID: " + it->first + ", ";
		result+=it->second.getString();
	}

	result+= "Non-deterministic Pushdown Automations\n";

	for (auto it = npdas.begin(); it != npdas.end(); ++it)
	{
		result += "ID: " + it->first + ", ";
		result += it->second.getString();
	}
	return result;
}
void Environment::setSecretForAssignments(FiniteStateAutomation& fsa)
{
	fsaSecret = fsa;
}
FiniteStateAutomation& Environment::GetSecretForAssignments()
{
	return fsaSecret;
}