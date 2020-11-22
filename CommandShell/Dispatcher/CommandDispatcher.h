#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "../../Environment/Environment.h"
#include "../Commands/Commands.h"

class CommandDispatcher
{

	std::map<std::string, Command*> registeredCommands;
public:
	CommandDispatcher();
	void registerCommand(const std::string& commandName, Command* command);
	std::string dispatch(const std::vector<std::string>& args);
	std::string dispatch(std::string);
	Environment env;
};

