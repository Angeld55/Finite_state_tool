#include "FSA\FiniteStateAutomation.h"

int main() 
{	
	//Example for NFA with reg. expr: (a(a+b)*b) + (b(a+b)*a)


	FiniteStateAutomation a("((((a)+(b)))*.(b))"); // (a(a+b)*b) + (b(a+b)*a)

	std::cout << a.Accept("abba") << std::endl; //false
	std::cout << a.Accept("abbb") << std::endl; //true
	std::cout << a.Accept("aaabbbbaaaaab") << std::endl; //false

	a.Minimize();
	
	a.Print();

	std::cout<<a.GetRegEx()<<std::endl;
   	
	return 0;
}
 