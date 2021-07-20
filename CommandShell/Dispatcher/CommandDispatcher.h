#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "../../Environment/AFL_Environment.h"
#include "../Commands/Commands.h"

class CommandDispatcher
{
	std::map<std::string, Command*> registeredCommands;
	void registerCommand(const std::string& commandName, Command* command);

public:
	CommandDispatcher();
	std::string dispatch(std::string);
	std::string dispatch(const std::vector<std::string>& args);
	void reset();

	AFL_Environment env;
};

