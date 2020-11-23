#pragma once
#include <iostream>
#include <vector>
class CommandDispatcher;
class Command
{
public:
	Command(CommandDispatcher& dispatcher) : dispatcher(dispatcher){}
	CommandDispatcher& dispatcher;
	virtual ~Command() {}
	virtual std::string execute(const std::vector<std::string>& args) = 0;
};

class AssignCommand : public Command
{
	//A = B
	//A = DET B
public:
	AssignCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);

};
class UnaryCommand : public Command
{
    //A = DET B, A = MIN B , A = COMPL B
	public:
	UnaryCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
class TransitionCommand : public Command
{
	// arc A 0 3 b, arc P 0, 'a', '#', 0, "A#"
	public:
	TransitionCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
class AddStateCommand : public Command
{
	// add_state A
	public:
	AddStateCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
class MakeFinalStateCommand : public Command
{
	// make_final A 3
	public:
	MakeFinalStateCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
class AcceptsCommand : public Command
{
	// accepts A aabba
	public:
	AcceptsCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
class CreationCommand : public Command
{
	// FSA test  FSA aut a*b*
	public:
	CreationCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
class PrintCommand : public Command
{
	//print A
public:
	PrintCommand(CommandDispatcher& dispatcher) : Command(dispatcher){}
	std::string execute(const std::vector<std::string>& args);
};
