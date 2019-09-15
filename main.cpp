#include "FiniteStateAutomation.hpp"

int main() 
{	
	FiniteStateAutomation a("((a).(b))");
	a.Print();
	a.Minimize();
	a.Print();

   	return 0;
}
