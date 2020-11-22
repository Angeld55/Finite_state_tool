#pragma once
#include "CommandShell.hpp"
class AssignCommand : public Command
{
	//A = B
	//A = DET B
	std::string execute(const std::vector<std::string>& args)
	{
		if (args.size() == 4) // A = DET B, A = MIN B, 
		{
			std::vector<std::string> newCommandArgs = args;
			newCommandArgs.erase(newCommandArgs.begin());
			newCommandArgs.erase(newCommandArgs.begin());

			newCommandArgs.push_back("_assign");
			dispatcher.dispatch(newCommandArgs);

			dispatcher.env.registerFSA(args[0], dispatcher.env.GetSecretForAssignments());
			return "";
		}
		if (args.size() != 3)
			return "Error! Invalid command!";
		// A = B
		FiniteStateAutomation fsaLeft;
		FiniteStateAutomation fsaRight;
		if (dispatcher.env.getFSA(args[0], fsaLeft) == 0)
		{
			if (dispatcher.env.getFSA(args[2], fsaRight) == 0)
			{
				dispatcher.env.registerFSA(args[0], fsaRight);
				return "";
			}

			else
				return "Error! No FSA with ID:" + args[2];

		}
		else
		{
			NPDA npdLeft;
			NPDA npdright;
			if (dispatcher.env.getNPDA(args[0], npdLeft) == 0)
			{
				if (dispatcher.env.getNPDA(args[2], npdright) == 0)
				{
					dispatcher.env.registerNPDA(args[0], npdright);
					return "";
				}
				else
					return "Error! No NPDA with ID:" + args[2];
			}
			else
				return "Error! No FSA/NPDA with ID: " + args[2];
		}
		return "Error! No";
	}
};