#include "Commands.h"
#include "../Dispatcher/CommandDispatcher.h"
std::string AssignCommand::execute(const std::vector<std::string>& args)
{
	// A = DET B, A = MIN B,
	if (args.size() == 4) 
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

std::string DetMinTotCommand::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2 || args.size() != 3)
		return "Error! Invalid command! ";

	FiniteStateAutomation fsa;
	if (dispatcher.env.getFSA(args[1], fsa) == 0)
	{
		if (args.size() == 2)
		{
			if (args[0] == "det")
				fsa.makeDeterministic();
			else if (args[0] == "min")
				fsa.minimize();
			else if (args[0] == "tot")
				fsa.makeTotal();
			dispatcher.env.registerFSA(args[1], fsa);
			return "";
		}
		else if (args.size() == 3 && args[2] == "_assign") //Det A _assign
		{
			FiniteStateAutomation fsaTemp = fsa;
				
			if (args[0] == "det")
				fsaTemp.makeDeterministic();
			else if (args[0] == "min")
				fsaTemp.minimize();
			else if (args[0] == "tot")
				fsaTemp.makeTotal();

			dispatcher.env.setSecretForAssignments(fsaTemp);
			return "";
		}
		else
			return "Error! Invalid command!";
	}
	else
		return "Error! No FSA/NPDA with ID: " + args[2];

}
bool onlyDigits(const std::string &str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit); 
}
std::string TransitionCommand::execute(const std::vector<std::string>& args)
{
	FiniteStateAutomation fs;
	NPDA pa;
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		if (args.size() == 5)
		{
			if (args[4].size() != 1)
				return "Transitions should have only one symbol!";
			if (!onlyDigits(args[2]) || !onlyDigits(args[3]))
				return "Invalid states!";
			bool res = fs.addTransition(atoi(args[2].c_str()), atoi(args[3].c_str()), args[4][0]);
			return  res ? "" : "Error! One of the states given does not exists!";
		}
		else
			return "Error! Invalid command!";
	}
	else if (dispatcher.env.getNPDA(args[1], pa) == 0)
	{
		if (args.size() == 7)
		{
			if (!onlyDigits(args[2]) || !onlyDigits(args[5]))
				return "Invalid states!";
			if (args[3].size() != 1 || args[4].size() != 1)
				return "Transitions should have only one symbol!";
			pa.addTransition(atoi(args[2].c_str()), args[3][0], args[4][0], atoi(args[5].c_str()), args[6]);
		}
		else
			return "Error! Invalid command!";
	}
}

std::string AddStateCommand::execute(const std::vector<std::string>& args)  // add_state A
{
	FiniteStateAutomation fs;
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		fs.addState();
		return "";
	}
	else
		return "Error! No FSA with ID:" + args[1];
}

std::string MakeFinalStateCommand::execute(const std::vector<std::string>& args)  // make_final A 3
{
	FiniteStateAutomation fs;
	NPDA pa;
	if (args.size() != 3 || !onlyDigits(args[1]))
		return "Error! Invalid command!";
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		bool res = fs.makeStateFinal(atoi(args[2].c_str()));
		return res ? "" : "Error! No such state!";
	}
	else if (dispatcher.env.getNPDA(args[1], pa) == 0)
	{
		bool res =  pa.makeFinal(atoi(args[2].c_str()));
		return res ? "" : "Error! No such state!";
	}
	else
		return "Error! No FSA with ID:" + args[1];
}
std::string AcceptsCommand::execute(const std::vector<std::string>& args)  
{
	FiniteStateAutomation fs;
	NPDA pa;
	if (args.size() != 3)
		return "Error! Invalid command!";
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		bool res = fs.accepts(args[2].c_str());
		return res ? "True!" : "False!";
	}
	else if (dispatcher.env.getNPDA(args[1], pa) == 0)
	{
		bool res = pa.accepts(args[2].c_str());
		return res ? "True!" : "False!";
	}
}

std::string CreationCommand::execute(const std::vector<std::string>& args)
{
	if (args[0] == "FSA")
	{
		if (args.size() == 3)
		{
			std::string regex = args[2];
			dispatcher.env.registerFSA(args[1], FiniteStateAutomation(regex.c_str()));
			
		}
		else
			dispatcher.env.registerFSA(args[1], FiniteStateAutomation());
		return "";
	}
	else if (args[0] == "NPDA")
	{
		if (args.size() == 2)
		{
			std::string CFG = args[2];
			//dispatcher->env.registerFSA(args[1], NPDA(CFG));
		}
		else
			dispatcher.env.registerNPDA(args[1], NPDA());
		return "";
	}
	return "";
}