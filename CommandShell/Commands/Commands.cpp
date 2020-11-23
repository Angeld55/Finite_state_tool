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
		std::string res = dispatcher.dispatch(newCommandArgs);
		if (res == "Succes!")
			dispatcher.env.registerFSA(args[0], dispatcher.env.GetSecretForAssignments());
		else
			return "Error! Invalid command! (Error 1100)";
		return "Succesfull operation on the FSA!";
	}
	// A = UNION B C
	if (args.size() == 5)
	{
		FiniteStateAutomation* fsaLeft = nullptr;
		FiniteStateAutomation* fsaRight = nullptr;
		if (dispatcher.env.getFSA(args[3], fsaLeft) == 0 && dispatcher.env.getFSA(args[4], fsaRight) == 0)
		{
			if (args[2] == "union")
			{
				FiniteStateAutomation res = Union(*fsaLeft, *fsaRight);
				dispatcher.env.registerFSA(args[0], res);
				return "Succesfull operation on the FSA!";

			}
			else if (args[2] == "intersect")
			{
				FiniteStateAutomation res = InterSection(*fsaLeft, *fsaRight);
				dispatcher.env.registerFSA(args[0], res);
				return "Succesfull operation on the FSA!";
			}
			else if (args[2] == "concat")
			{
				FiniteStateAutomation res = Concat(*fsaLeft, *fsaRight);
				dispatcher.env.registerFSA(args[0], res);
				return "Succesfull operation on the FSA!";
			}
			else
				return "Error! Invalid command! (Error 1110)";
		
		}
		else
			return "Error! One of the FSA-s doesn't exists!";
	}
	if (args.size() != 3)
		return "Error! Invalid command! (Error 1101)";
	// A = B
	FiniteStateAutomation* fsaLeft = nullptr;
	FiniteStateAutomation* fsaRight = nullptr;

	if (dispatcher.env.getFSA(args[2], fsaRight) == 0)
	{
		dispatcher.env.registerFSA(args[0], *fsaRight);
		return "";
	}
	else
	{
		NPDA* npdLeft = nullptr;
		NPDA* npdright = nullptr;
	
		if (dispatcher.env.getNPDA(args[2], npdright) == 0)
		{
			dispatcher.env.registerNPDA(args[0], *npdright);
			return "";
		}
		else
			return "Error! No FSA/NPDA with ID: " + args[2];
	}
	return "Error! Invalid command! (Error 1102)";
}

std::string UnaryCommand::execute(const std::vector<std::string>& args)
{

	//DET A
	// _assign DET A
	if (args.size() != 2 && args.size() != 3)
		return "Error! Invalid command! (Error 1103)";

	FiniteStateAutomation* fsa = nullptr;
	if (dispatcher.env.getFSA(args[1], fsa) == 0)
	{
		if (args.size() == 2)
		{
			if (args[0] == "det")
			{
				fsa->makeDeterministic();
				dispatcher.env.registerFSA(args[1], *fsa);
			}
			else if (args[0] == "min")
			{
				fsa->minimize();
				dispatcher.env.registerFSA(args[1], *fsa);
			}
			else if (args[0] == "tot")
			{
				fsa->makeTotal();
				dispatcher.env.registerFSA(args[1], *fsa);
			}
			else if (args[0] == "reverse")
			{
				FiniteStateAutomation reverse = Reverse(*fsa);
				dispatcher.env.registerFSA(args[1], reverse);
			}
			else if (args[0] == "compl")
			{
				FiniteStateAutomation compl = Complement(*fsa);
				dispatcher.env.registerFSA(args[1], compl);
			}
			else if (args[0] == "star")
			{
				FiniteStateAutomation star = Complement(*fsa);
				dispatcher.env.registerFSA(args[1], star);
			}
			else
				return "Error! Invalid command! (Error 1104)";
			
			return "Operation succesfully executed!";
		}
		else if (args.size() == 3 && args[2] == "_assign") //Det A _assign
		{
				
			if (args[0] == "det")
			{
				FiniteStateAutomation fsaTemp = *fsa;
				fsaTemp.makeDeterministic();
				dispatcher.env.setSecretForAssignments(fsaTemp);
			}
			else if (args[0] == "min")
			{
				FiniteStateAutomation fsaTemp = *fsa;
				fsaTemp.minimize();
				dispatcher.env.setSecretForAssignments(fsaTemp);
			}
			else if (args[0] == "tot")
			{
				FiniteStateAutomation fsaTemp = *fsa;
				fsaTemp.makeTotal();
				dispatcher.env.setSecretForAssignments(fsaTemp);
			}
			else if (args[0] == "reverse")
			{
				FiniteStateAutomation revesed = Reverse(*fsa);
				dispatcher.env.setSecretForAssignments(revesed);
			}
			else if (args[0] == "compl")
			{
				FiniteStateAutomation compl = Complement(*fsa);
				dispatcher.env.setSecretForAssignments(compl);
			}
			else if (args[0] == "star")
			{
				FiniteStateAutomation star = KleeneStar(*fsa);
				dispatcher.env.setSecretForAssignments(star);
			}
			else
				return "Error! Invalid command! (Error: 1111)";
			
			return "Succes!"; //flag for the assign function
		}
		else
			return "Error! Invalid command! (Error 1105)";
	}
	else
		return "Error! No FSA with ID: " + args[2];

}
bool onlyDigits(const std::string &str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit); 
}
std::string TransitionCommand::execute(const std::vector<std::string>& args)
{
	// arc A 0 3 b,   arc P 0, 'a', '#', 0, "A#"
	FiniteStateAutomation* fs = nullptr;
	NPDA* pa = nullptr;
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		if (args.size() == 5)
		{
			if (args[4].size() != 1)
				return "Transitions should have only one symbol!";
			if (!onlyDigits(args[2]) || !onlyDigits(args[3]))
				return "Invalid states!";
			bool res = fs->addTransition(atoi(args[2].c_str()), atoi(args[3].c_str()), args[4][0]);
			return  res ? "Trasition added sucessfully!" : "Error! One of the states given does not exists!";
		}
		else
			return "Error! Invalid command! (Error 1106)";
	}
	else if (dispatcher.env.getNPDA(args[1], pa) == 0)
	{
		if (args.size() == 7)
		{
			if (!onlyDigits(args[2]) || !onlyDigits(args[5]))
				return "Invalid states!";
			if (args[3].size() != 1 || args[4].size() != 1)
				return "Transitions should have only one symbol!";
			pa->addTransition(atoi(args[2].c_str()), args[3][0], args[4][0], atoi(args[5].c_str()), args[6]);
			return "Trasition added sucessfully!";
		}
		else
			return "Error! Invalid command! (Error 1107)";
	}
	return "Error! No FSA/PDA with ID: " + args[1];
}

std::string AddStateCommand::execute(const std::vector<std::string>& args)  // add_state A
{
	FiniteStateAutomation* fs = nullptr;
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		fs->addState();
		return "State added successfully!";
	}
	else
		return "Error! No FSA with ID:" + args[1];
}

std::string MakeFinalStateCommand::execute(const std::vector<std::string>& args)  // make_final A 3
{
	FiniteStateAutomation* fs = nullptr;
	NPDA* pa = nullptr;
	if (args.size() != 3 || !onlyDigits(args[1]))
		return "Error! Invalid command! (Error 1109)";
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		bool res = fs->makeStateFinal(atoi(args[2].c_str()));
		return res ? "Success! The state is now final!" : "Error! No such state!";
	}
	else if (dispatcher.env.getNPDA(args[1], pa) == 0)
	{
		bool res =  pa->makeFinal(atoi(args[2].c_str()));
		return res ? "Success! The state is now final!" : "Error! No such state!";
	}
	else
		return "Error! No FSA with ID:" + args[1];
}
std::string AcceptsCommand::execute(const std::vector<std::string>& args)  
{
	FiniteStateAutomation* fs = nullptr;
	NPDA* pa = nullptr;
	if (args.size() != 3)
		return "Error! Invalid command!";
	if (dispatcher.env.getFSA(args[1], fs) == 0)
	{
		bool res = fs->accepts(args[2].c_str());
		return res ? "True!" : "False!";
	}
	else if (dispatcher.env.getNPDA(args[1], pa) == 0)
	{
		bool res = pa->accepts(args[2].c_str());
		return res ? "True!" : "False!";
	}
	return "Error! No FSA with ID:" + args[1];
}

std::string CreationCommand::execute(const std::vector<std::string>& args)
{
	if (args[0] == "fsa")
	{
		if (args.size() == 3)
		{
			std::string regex = args[2];
			dispatcher.env.registerFSA(args[1], FiniteStateAutomation(regex.c_str()));
			
		}
		else if (args.size() == 2)
			dispatcher.env.registerFSA(args[1], FiniteStateAutomation());
		else
			return "Error! Invalid FSA name!";
		return "FSA succesfully created!";
	}
	else if (args[0] == "npda")
	{
		if (args.size() == 2)
		{
			std::string CFG = args[2];
			//dispatcher->env.registerFSA(args[1], NPDA(CFG));
		}
		else
			dispatcher.env.registerNPDA(args[1], NPDA());
		return "NPDA succesfully created!";
	}
	return "Error! Invalid command! (Error: 1108)";
}
std::string PrintCommand::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2)
		return "Error! Invalid command! (Error: 1109)";
	FiniteStateAutomation* fs = nullptr;
	NPDA* pa = nullptr;
	if (dispatcher.env.getFSA(args[1], fs) == 0)
		return fs->getFullString();
	else if ((dispatcher.env.getNPDA(args[1], pa) == 0))
		return pa->getString();
	return  "Error! No FSA/PDA with ID: " + args[1];
}