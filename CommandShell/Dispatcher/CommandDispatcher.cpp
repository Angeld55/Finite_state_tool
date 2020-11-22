#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher()
{
	registerCommand("assign", new AssignCommand(*this));
	registerCommand("det", new DetMinTotCommand(*this));
	registerCommand("min", new DetMinTotCommand(*this));
	registerCommand("tot", new DetMinTotCommand(*this));
	registerCommand("arc", new TransitionCommand(*this));
	registerCommand("add_state", new AddStateCommand(*this));
	registerCommand("make_final", new MakeFinalStateCommand(*this));
	registerCommand("accepts", new TransitionCommand(*this));
	registerCommand("FSA", new CreationCommand(*this));
}

void CommandDispatcher::registerCommand(const std::string& commandName, Command* command)
{
	auto cmdPair = registeredCommands.find(commandName);
	if (cmdPair != registeredCommands.end())
	{
		// handle error: command already registered
	}
	else
		registeredCommands[commandName] = command;
}

std::string CommandDispatcher::dispatch(const std::vector<std::string>& args)
{
	if (!args.empty())
	{
		std::map<std::string, Command*>::const_iterator cmdPair = registeredCommands.find(args[0]);
		if (cmdPair == registeredCommands.end())
		{
			return "Error! Invalid command!";
		}
		else
		{
			cout << "here";
			try
			{
				Command* cmd = cmdPair->second;
				return cmd->execute(args);
			}
			catch (exception& e)
			{
				return "Unhandled error!";
			}
		}
	}
}
bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

void removeMultipleWhitespacesWithOne(std::string& str)
{
	std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
	str.erase(new_end, str.end());
}
std::string CommandDispatcher::dispatch(std::string input)
{
	removeMultipleWhitespacesWithOne(input);
	
	std::vector<std::string> args;
	std::string delimiter = " ";

	size_t pos = 0;
	std::string token;
	while ((pos = input.find(delimiter)) != std::string::npos) 
	{
		token = input.substr(0, pos);
		args.push_back(token);
		input.erase(0, pos + delimiter.length());
	}
	args.push_back(input);
	return dispatch(args);
}


