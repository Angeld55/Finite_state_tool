#include "FSA\FiniteStateAutomation.h"

int main() 
{	
	//Example for NFA with reg. expr: (a+b)*b


	FiniteStateAutomation a("((((a)+(b)))*.(b))"); // (a(a+b)*b)

	std::cout << a.Accept("abba") << std::endl; //false
	std::cout << a.Accept("abbb") << std::endl; //true
	std::cout << a.Accept("aaabbbbaaaaab") << std::endl; //true

	a.Minimize();
	
	a.Print();

	std::cout<<a.GetRegEx()<<std::endl;
   	
	return 0;
}
 
