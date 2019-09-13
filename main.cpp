#include "FiniteStateAutomation.hpp"

int main() 
{
	
	//With  regular expression (Have to use many brackets for now) *fix later
	FiniteStateAutomation A("((a).(((a)+(b)))*)");

	
	FiniteStateAutomation A2;//Automation with one state (for now)
	//Don't forget to put the letters for the alphabet. In the regular expression way of creating an automation, it does it automatically.
	A2.AddLetterToAlphabet('a');
	A2.AddLetterToAlphabet('b');

	A2.AddState();
	A2.AddTransition(0, 1, 'a');
	A2.AddTransition(1, 1, 'a');
	A2.AddTransition(1, 1, 'b');


   	return 0;
}
