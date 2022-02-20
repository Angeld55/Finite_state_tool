#include "../Finate_state_automation/FiniteStateAutomation.h"
#include "../Non-deterministic_pushdown_automaton/NPDA.h"

bool testFSACreateAutomationByRegex()
{
	FiniteStateAutomation f("a(a+b)*b+b(a+b)*a");

	std::string computation;
	if (!f.accepts("aab", computation))
		return false;

	if (!f.accepts("abbbbbaab", computation))
		return false;


	if (!f.accepts("babababaaa", computation))
		return false;

	if (f.accepts("aaaaaaaaaaa", computation))
		return false;

	if (f.accepts("bbbbb", computation))
		return false;


	if (f.accepts("", computation))
		return false;
	return true;
}

bool testFSATranstitionAddingAndDetAutomation()
{
	FiniteStateAutomation f;
	f.addState();
	f.addState();
	f.addState();
	f.addState();

	f.addTransition({ "0", "0", "a" });
	f.addTransition({ "0", "0", "b" });

	f.addTransition({ "0", "1", "a" });

	f.addTransition({ "1", "2", "a" });
	f.addTransition({ "1", "2", "b" });

	f.addTransition({ "2", "3", "a" });
	f.addTransition({ "2", "3", "b" });

	f.addTransition({ "3", "4", "a" });
	f.addTransition({ "3", "4", "b" });

	f.makeStateFinal(4);

	std::string computation;
	if (!f.accepts("aaab", computation))
		return false;

	if (f.accepts("bbabbbaab", computation))
		return false;

	if (!f.accepts("bbabbaaab", computation))
		return false;

	f.makeDeterministic();

	if (f.getStatesCount() != 16) // 2^4
		return false;

	if (!f.accepts("aaab", computation))
		return false;

	if (f.accepts("bbabbbaab", computation))
		return false;

	if (!f.accepts("bbabbaaab", computation))
		return false;
}


bool testFSAMin()
{
	FiniteStateAutomation f("a(a+b)*b+b(a+b)*a+a(a+b)*a+b(a+b)*b+$+a+b");

	std::string computation;
	if (!f.accepts("aab", computation))
		return false;

	if (!f.accepts("abbbbbaab", computation))
		return false;


	if (!f.accepts("babababaaa", computation))
		return false;

	if (!f.accepts("aaaaaaaaaaa", computation))
		return false;

	if (!f.accepts("", computation))
		return false;

	f.minimize();

	if (f.getStatesCount() != 1)
		return false;

	if (!f.accepts("aab", computation))
		return false;

	if (!f.accepts("abbbbbaab", computation))
		return false;


	if (!f.accepts("babababaaa", computation))
		return false;

	if (!f.accepts("aaaaaaaaaaa", computation))
		return false;

	if (!f.accepts("", computation))
		return false;

	return true;
}

bool testComplementAutomation()
{
	FiniteStateAutomation f("a(ba+b)*");
	
	FiniteStateAutomation c = Complement(f);

	std::string computation;
	if (!c.accepts("bbb", computation))
		return false;

	if (!c.accepts("bb", computation))
		return false;


	if (!c.accepts("baabababa", computation))
		return false;

	if (!c.accepts("babababa", computation))
		return false;

	if (!c.accepts("abbaa", computation))
		return false;

	if (c.accepts("abbaba", computation))
		return false;


	if (c.accepts("abbababbbbb", computation))
		return false;


	if (!c.accepts("abbababbbaabb", computation))
		return false;

	return true;
}

bool testIntersection()
{
	FiniteStateAutomation f("a(a+b)*a+a");

	FiniteStateAutomation f2("((a+b)(a+b))*");

	FiniteStateAutomation res = InterSection(f, f2);

	std::string computation;
	if (!res.accepts("abba", computation))
		return false;

	if (!res.accepts("abaaba", computation))
		return false;


	if (!res.accepts("aa", computation))
		return false;

	if (res.accepts("aab", computation))
		return false;

	if (res.accepts("aabbaba", computation))
		return false;

	if (res.accepts("aba", computation))
		return false;

	return true;
}

bool testTotal()
{
	{
		FiniteStateAutomation f;

		f.addState();

		f.addTransition({ "0", "1", "a" });
		f.addTransition({ "1", "0", "b" });

		if (f.isTotal())
			return false;

		f.makeTotal();

		if (!f.isTotal())
			return false;

		if (f.getStatesCount() != 3)
			return false;
	}

	{
		FiniteStateAutomation f;

		f.addState();

		f.addTransition({ "0", "1", "a" });
		f.addTransition({ "1", "0", "a" });

		if (!f.isTotal())
			return false;

		f.makeTotal();

		if (!f.isTotal())
			return false;

		if (f.getStatesCount() != 2)
			return false;
	}
	return true;
}



bool testNDPA()
{
	NPDA PA(3); //3 initial states

	PA.makeStateFinal(2);

	PA.addTransition({ "0", "a", "#", "0", "A#" });

	PA.addTransition({ "0", "b", "#", "0", "B#" });
	PA.addTransition({ "0", "$", "#", "2", "$" });

	PA.addTransition({ "0", "a", "A", "0", "AA" });
	PA.addTransition({ "0", "a", "A", "1", "$" });

	PA.addTransition({ "0", "b", "B", "0", "BB" });
	PA.addTransition({ "0", "b", "B", "1", "$" });

	PA.addTransition({ "0", "b", "A", "0", "BA" });
	PA.addTransition({ "0", "a", "B", "0", "AB" });

	PA.addTransition({ "1", "a", "A", "1", "$" });
	PA.addTransition({ "1", "b", "B", "1", "$" });

	PA.addTransition({ "1", "$", "#", "2", "$" });

	string computation;
	if (!PA.accepts("abba", computation))
		return false;
	if (PA.accepts("abbb", computation))
		return false;
	if (!PA.accepts("aaabbbbbbaaa", computation))
		return false;

	return true;
}

bool testCFG()
{
	ContextFreeGrammar cfg;
	cfg.grammarRules.push_back("S->aSc|B");
	cfg.grammarRules.push_back("B->bB|$");

	NPDA PA2(cfg);

	std::string computation;
	if (!PA2.accepts("abc", computation))
		return false;

	if (!PA2.accepts("aaaaaabbbbcccccc", computation))
		return false;

	if (PA2.accepts("abcc", computation))
		return false;

	return true;
}
int main()
{
	std::cout << "Test (FSA): creating automation by a regex" << std::endl;
	if (testFSACreateAutomationByRegex())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;

	std::cout << "Test (FSA): transition adding and det" << std::endl;
	if (testFSATranstitionAddingAndDetAutomation())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;

	std::cout << "Test (FSA): minimization" << std::endl;
	if (testFSAMin())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;

	std::cout << "Test (FSA): complement automation" << std::endl;
	if (testComplementAutomation())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;

	std::cout << "Test (FSA): intersection of automations" << std::endl;
	if (testIntersection())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;


	std::cout << "Test (FSA): Making a total automation" << std::endl;
	if (testTotal())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;


	std::cout << "Test (NPDA): Construction of a NPDA" << std::endl;
	if (testNDPA())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;

	std::cout << "Test (CFG): NPDA by a CFG" << std::endl;
	if (testCFG())
		std::cout << "Passed!" << std::endl;
	else
		std::cout << "Failed!" << std::endl;

	return 0;
}
