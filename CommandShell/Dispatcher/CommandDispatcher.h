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

	CommandDispatcher(const CommandDispatcher&) = delete;
	CommandDispatcher& operator=(const CommandDispatcher&) = delete;
	std::string dispatch(const std::string&);
	std::string dispatch(const std::vector<std::string>&);
	void reset();

	~CommandDispatcher();

	AFL_Environment env; //TODO: Remove that
};

