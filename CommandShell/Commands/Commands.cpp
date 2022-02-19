#include "Commands.h"
#include "../Dispatcher/CommandDispatcher.h"
+#include <numeric>
+#include <algorithm>
+#include <iterator>
+#include <iostream>
+#include <functional>


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
			dispatcher.env.registerAutomation(args[0], dispatcher.env.GetSecretForAssignments());
		else
			return "Error! Invalid command! (Error 1100)";
		return "Successfully executed operation on the FSA!";
	}
	// A = UNION B C
	if (args.size() == 5)
	{
		AutomationBase* lhs = nullptr;
		AutomationBase* rhs = nullptr;
		if (dispatcher.env.getAutomation(args[3], lhs) == 0 && dispatcher.env.getAutomation(args[4], rhs) == 0)
		{
			FiniteStateAutomation* fsaLeft;
			FiniteStateAutomation* fsaRight;
			if (lhs->getType() == AutomationBase::AutomationType::FSA && rhs->getType() == AutomationBase::AutomationType::FSA)
			{
				fsaLeft = static_cast<FiniteStateAutomation*>(lhs);
				fsaRight = static_cast<FiniteStateAutomation*>(rhs);
			}
			else
				return "Error! One of the FSA-s doesn't exists!";

			if (args[2] == "union")
			{
				FiniteStateAutomation* res = new FiniteStateAutomation(Union(*fsaLeft, *fsaRight));
				dispatcher.env.registerAutomation(args[0], res);
			}
			else if (args[2] == "intersect")
			{
				FiniteStateAutomation* res = new FiniteStateAutomation(InterSection(*fsaLeft, *fsaRight));
				dispatcher.env.registerAutomation(args[0], res);
			}
			else if (args[2] == "concat")
			{
				FiniteStateAutomation* res = new FiniteStateAutomation(Concat(*fsaLeft, *fsaRight));
				dispatcher.env.registerAutomation(args[0], res);
			}
			else
				return "Error! Invalid command! (Error 1110)";
			return "Successfully executed operation on the FSAs!";
		}
		else
			return "Error! One of the FSA-s doesn't exists!";
	}
	if (args.size() != 3)
		return "Error! Invalid command! (Error 1101)";
	// A = B
	AutomationBase* fsaLeft = nullptr;
	AutomationBase* fsaRight = nullptr;

	if (dispatcher.env.getAutomation(args[2], fsaRight) == 0)
	{
		dispatcher.env.registerAutomation(args[0], fsaRight->clone());
		return "Assignment succesfull!";
	}
	else
		return "Error! No FSA/NPDA with ID: " + args[2];
	return "Error! Invalid command! (Error 1102)";
}

std::string UnaryCommand::execute(const std::vector<std::string>& args)
{

	//DET A
	// _assign DET A
	if (args.size() != 2 && args.size() != 3)
		return "Error! Invalid command! (Error 1103)";

	AutomationBase* ptr = nullptr;
	if (dispatcher.env.getAutomation(args[1], ptr) == 0)
	{
		FiniteStateAutomation* fsa = static_cast<FiniteStateAutomation*> (ptr);
		if (args.size() == 2)
		{
			if (args[0] == "det")
				fsa->makeDeterministic();

			else if (args[0] == "min")
				fsa->minimize();
		
			else if (args[0] == "tot")
				fsa->makeTotal();

			else if (args[0] == "reverse")
			{
				FiniteStateAutomation* reverse = new FiniteStateAutomation(Reverse(*fsa)); //TODO// remove unnecessary copy
				dispatcher.env.registerAutomation(args[1], reverse);
			}
			else if (args[0] == "compl")
			{
				FiniteStateAutomation* comple = new FiniteStateAutomation(Complement(*fsa));
				dispatcher.env.registerAutomation(args[1], comple);
			}
			else if (args[0] == "star")
			{
				FiniteStateAutomation* star = new FiniteStateAutomation(KleeneStar(*fsa));
				dispatcher.env.registerAutomation(args[1], star);
			}
			else
				return "Error! Invalid command! (Error 1104)";
			
			return "Operation succesfully executed!";
		}
		else if (args.size() == 3 && args[2] == "_assign") //Det A _assign
		{
				
			if (args[0] == "det")
			{
				FiniteStateAutomation* fsaTemp = new FiniteStateAutomation(*fsa);
				fsaTemp->makeDeterministic();
				dispatcher.env.setSecretForAssignments(fsaTemp);
			}
			else if (args[0] == "min")
			{
				FiniteStateAutomation* fsaTemp = new FiniteStateAutomation(*fsa);
				fsaTemp->minimize();
				dispatcher.env.setSecretForAssignments(fsaTemp);
			}
			else if (args[0] == "tot")
			{
				FiniteStateAutomation* fsaTemp = new FiniteStateAutomation(*fsa);
				fsaTemp->makeTotal();
				dispatcher.env.setSecretForAssignments(fsaTemp);
			}
			else if (args[0] == "reverse")
			{
				FiniteStateAutomation* reversed = new FiniteStateAutomation(Reverse(*fsa));
				dispatcher.env.setSecretForAssignments(reversed);
			}
			else if (args[0] == "compl")
			{
				FiniteStateAutomation* complement = new FiniteStateAutomation(Complement(*fsa));
				dispatcher.env.setSecretForAssignments(complement);
			}
			else if (args[0] == "star")
			{
				FiniteStateAutomation* star = new FiniteStateAutomation(KleeneStar(*fsa));
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

	AutomationBase* ptr = nullptr;

	if (dispatcher.env.getAutomation(args[1], ptr) == 0)
	{
		std::vector<std::string> arc = args;
		arc.erase(arc.begin(), arc.begin() + 2);
		int res = ptr->addTransition(arc);
		if (res == 0)
			return "Trasition added sucessfully!";

		switch (res)
		{
		case -1: return "Error! Invalid command! (Error 1106)";
		case -2: return "Error! The states should contain only digits!";
		case -3: return "Error! One of the states given does not exists!";
		case -4: return "Error! Invalid arc syntax!";
		default: return "Error! Invalid command! (Error 1107)";
		}

	}
	return "Error! No FSA with ID: " + args[2];

}

std::string AddStateCommand::execute(const std::vector<std::string>& args)  // add_state A
{
	AutomationBase* ptr = nullptr;

	if (dispatcher.env.getAutomation(args[1], ptr) == 0)
	{
		ptr->addState();
		return "State added successfully!";
	}
	return "Error! No FSA with ID: " + args[1];
}

std::string MakeFinalStateCommand::execute(const std::vector<std::string>& args)  // make_final A 3
{
	AutomationBase* aut = nullptr;
	if (args.size() != 3 || !onlyDigits(args[2]))
		return "Error! Invalid command! (Error 1109)";
	if (dispatcher.env.getAutomation(args[1], aut) == 0)
	{
		int res = aut->makeStateFinal(atoi(args[2].c_str()));
		switch (res)
		{
		case 0: return "Success! The state is now final!";  break;
		case -1: return "Error! The state is already final!"; break;
		case -2: return "Error! No such state!"; break;
		default: return "Invalid command! (Error 1156)";
		}
	}
		return "Error! No FSA with ID:" + args[1];
}
std::string AcceptsCommand::execute(const std::vector<std::string>& args)  
{
	//accepts A aaabb full
	AutomationBase* ptr = nullptr;

	if (args.size() < 3)
		return "Error! Invalid command!";
	
	if (dispatcher.env.getAutomation(args[1], ptr) == 0)
	{
		bool shouldPrintComputation = args.size() >= 4 && args[3] == "full";
		std::string computation;

		bool res = ptr->accepts(args[2], computation, shouldPrintComputation);
		
		string resultStr = (res ? "True!" : "False!") + computation;

		return resultStr;
	}
	return "Error! No FSA/NPDA with ID:" + args[1];
}

std::string CreationCommand::execute(const std::vector<std::string>& args)
{
	if (args.size()<2)
		return "Error! Invalid command!";
	//fsa test ab*
	if (args[0] == "fsa")
	{
		if (args.size() == 3)
		{
			std::string regex = args[2];
			dispatcher.env.registerAutomation(args[1], new FiniteStateAutomation(regex.c_str()));
			
		}
		else if (args.size() == 2)
			dispatcher.env.registerAutomation(args[1], new FiniteStateAutomation());
		else
			return "Error! Invalid command! (Error 1115)";
		return "FSA succesfully created!";
	}
	else if (args[0] == "npda")
	{
		if (args.size() == 2)
		{
			dispatcher.env.registerAutomation(args[1], new NPDA());
			return "NPDA succesfully created!";
		}
		return "Error! Invalid command! (Error: 1118)";
	}
	else if (args[0] == "cfg")
	{ // npda test cfg S->aSb|$
	
		if (args.size() <= 2)
			return "Error! Invalid command! (Error: 1120)";

		ContextFreeGrammar cfg;

		for (int i = 2; i < args.size(); i++)
			cfg.grammarRules.push_back(args[i]);
		dispatcher.env.registerAutomation(args[1], new NPDA(cfg));

		return "NPDA(from CFG) succesfully created!";
	

	}
	return "Error! Invalid command! (Error: 1108)";
}
std::string PrintCommand::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2)
		return "Error! Invalid command! (Error: 1109)";
	AutomationBase* fs = nullptr;

	if (dispatcher.env.getAutomation(args[1], fs) == 0)
	{
		if (fs->getType() == AutomationBase::AutomationType::FSA) //TODO. Remove this casting
			return static_cast<FiniteStateAutomation*>(fs)->getFullString();
		return fs->getString();
	}
	return  "Error! No FSA/PDA with ID: " + args[1];
}
std::string RegexCommand::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2)
		return "Error! Invalid command! (Error: 1114)";
	AutomationBase* fs = nullptr;
	if (dispatcher.env.getAutomation(args[1], fs) == 0)
	{ 
		if (fs->getType() == AutomationBase::AutomationType::FSA)
			return std::string((static_cast<FiniteStateAutomation*>(fs))->getRegEx().c_str());
	}
	return  "Error! No FSA with ID: " + args[1];
}

std::string EnvironmentCommand::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2)
		return "Error! Invalid command! (Error: 1116)";
	return dispatcher.env.toStringAutomations();
}
std::string VisualizeCommand::execute(const std::vector<std::string>& args)
{
	AutomationBase* aut = nullptr;
	if (args.size() != 2)
		return "Error! Invalid command! (Error: 1122)";
	if (dispatcher.env.getAutomation(args[1], aut) == 0)
	{
		if (!dispatcher.env.vis.isOkey())
		{
			dispatcher.env.vis.init();
		}

		if (!dispatcher.env.vis.isOkey())
			return "Error with the visualization!";

		dispatcher.env.vis.visualize(aut, args[1] + ".html");
		return "Visualization succesfull!";
	}
	else
		return "Error! No FSA with ID:" + args[0];


}
