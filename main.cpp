#include "Finate_state_automation\FiniteStateAutomation.h"
#include "Non-deterministic_pushdown_automaton\NPDA.h"

int main() 
{	
	//Example for NFA with reg. expr: (a+b)*b

	FiniteStateAutomation a("((((a)+(b)))*.(b))"); // ((a+b)*b)

	std::cout << a.Accepts("abba") << std::endl; //false
	std::cout << a.Accepts("abbb") << std::endl; //true
	std::cout << a.Accepts("aaabbbbaaaaab") << std::endl; //true

	a.Minimize();
	a.Print();
	std::cout<<a.GetRegEx()<<std::endl;
   	
	// Example for Nondeterministic pushdown automata for { ww^rev | w in {a,b}* }

	NPDA PA(3); //3 initial states

	PA.MakeFinal(2);
	PA.AddTransition(0, 'a', '#', 0, "A#");
	PA.AddTransition(0, 'b', '#', 0, "B#");
	PA.AddTransition(0, '$', '#', 2, "$");

	PA.AddTransition(0, 'a', 'A', 0, "AA");
	PA.AddTransition(0, 'a', 'A', 1, "$");

	PA.AddTransition(0, 'b', 'B', 0, "BB");
	PA.AddTransition(0, 'b', 'B', 1, "$");

	PA.AddTransition(0, 'b', 'A', 0, "BA");
	PA.AddTransition(0, 'a', 'B', 0, "AB");

	PA.AddTransition(1, 'a', 'A', 1, "$");
	PA.AddTransition(1, 'b', 'B', 1, "$");

	PA.AddTransition(1, '$', '#', 2, "$");

	std::cout << PA.Accepts("abba") << std::endl; //true
	std::cout << PA.Accepts("abbb") << std::endl; //false
	std::cout << PA.Accepts("aaabbbbbbaaa") << std::endl; //true



	//Example Nondeterministic pushdown automata  from a context-free grammar
	// S->aSc | B
	// B->bB | $
	// L(S) = { a^n b^k c^n | n,k \in N}
	ContextFreeGrammar cfg;
	cfg.grammarRules.push_back("S->aSc|B");
	cfg.grammarRules.push_back("B->bB|$");

	NPDA PA2(cfg);

	std::cout << PA2.Accepts("abc", true) << std::endl; //true
	std::cout << PA2.Accepts("aaaaaabbbbcccccc") << std::endl; //true
	std::cout << PA2.Accepts("abcc") << std::endl; //false

	return 0;
}
 
