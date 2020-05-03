#include "FSA\FiniteStateAutomation.h"

int main() 
{	

	FiniteStateAutomation a("((((a).(((a)+(b)))*).(b))+(((b).(((a)+(b)))*).(a)))"); // (a(a+b)*b) + (b(a+b)*a)

	a.Minimize();
	
	a.Print();

	std::cout<<a.GetRegEx()<<std::endl;
   	return 0;
}
 