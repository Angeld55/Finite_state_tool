#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include "../Command_shell/Dispatcher/CommandDispatcher.h"

void trim(string& str) 
{
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0, str.find_first_not_of(typeOfWhitespaces));
}

bool loadConfiguration(CommandDispatcher& dispatcher, const std::string& configFile, std::string& dispatcherFinalResponse)
{
	ifstream infile(configFile);

	if (!infile.is_open())
	{
		dispatcherFinalResponse = "The file doesn't exists!";
		return false;
	}

	std::string line;
	while (std::getline(infile, line))
	{
		trim(line);
		std::string request = line.substr(0, line.find("//", 0)); //remove comments

		if (request.length() == 0)
			continue;
		std::string response = dispatcher.dispatch(request);

		if (response.find("Error", 0) == 0)
		{
			dispatcherFinalResponse = response;
			return false;
		}
	}
	dispatcherFinalResponse = "Configuration file successfuly loaded!";
	return true;
}

void runAFL()
{
	CommandDispatcher dispatcher;
	while (1)
	{
		cout << ">";
		std::string request;
		std::getline(cin, request);
		
		std::string response;
		trim(request);

		if (request.rfind("load", 0) == 0) //load saved configuration
		{
			loadConfiguration(dispatcher, request.substr(5), response);
			cout << response << endl;
			continue;
		}
		else if(request == "exit" || request == "quit")
			return;
		
		response = dispatcher.dispatch(request);
		std::cout << response << std::endl;


	}
}
int main() 
{
	runAFL();
	return 0;
}
 
