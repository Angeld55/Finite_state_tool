#include "AFL_Environment.h"

int AFL_Environment::registerFSA(const std::string& key, const FiniteStateAutomation& fsa)
{
	auto autPair= fsas.find(key);
	fsas[key] = fsa;
	return 0;

}
int AFL_Environment::registerNPDA(const std::string& key, const NPDA& fsa)
{
	auto autPair = npdas.find(key);

	npdas[key] = fsa;
	return 0;

}
int AFL_Environment::getNPDA(const std::string& key, NPDA*& npda)
{
	auto autPair = npdas.find(key);
	if (autPair != npdas.end())
	{
		npda = &npdas[key];
		return 0;
	}
	else
		return -1;//does not exists
}
int AFL_Environment::getFSA(const std::string& key, FiniteStateAutomation*& fsa)
{
	auto autPair = fsas.find(key);
	if (autPair != fsas.end())
	{
		fsa = &fsas[key];
		return 0;
	}
	else
		return -1;//does not exists
}
std::string AFL_Environment::toStringFSA()
{
	std::string result = "Finite-State Automations:\n";

	for (auto it = fsas.begin(); it != fsas.end(); ++it)
	{
		result += "ID: " + it->first + ", ";
		result+=it->second.getString();
	}
	return result;
}
std::string AFL_Environment::toStringNPDA()
{
	std::string result = "Non-deterministic Pushdown Automations: TBD\n";

	for (auto it = npdas.begin(); it != npdas.end(); ++it)
	{
		result += "ID: " + it->first + ", ";
		result += it->second.getString();
	}
	return result;
}
void AFL_Environment::setSecretForAssignments(FiniteStateAutomation& fsa)
{
	fsaSecret = fsa;
}
FiniteStateAutomation& AFL_Environment::GetSecretForAssignments()
{
	return fsaSecret;
}

