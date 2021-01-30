#include <iostream>
#include <vector>
#include <string>
#include "CommandShell\Dispatcher\CommandDispatcher.h"
using namespace std;


void npdaTest()
{
	CommandDispatcher dispatecher;
	std::string request = "npda even_palindromes";
	cout << dispatecher.dispatch(request) << endl;

	request = "add_state even_palindromes";
	cout << dispatecher.dispatch(request) << endl;
	cout << dispatecher.dispatch(request) << endl;

	request = "make_final even_palindromes 2";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 a # 0 A#";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 b # 0 B#";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 $ # 2 $";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 a A 0 AA";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 a A 0 $";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 b B 0 BB";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 b B 0 $";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 a B 0 AB";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 0 b A 0 BA";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 1 b B 1 $";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 1 a A 1 $";
	cout << dispatecher.dispatch(request) << endl;

	request = "arc even_palindromes 1 $ # 2 $";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts even_palindromes aabbbbaa";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts even_palindromes aabbbba";
	cout << dispatecher.dispatch(request) << endl;
}

void fsaTest()
{
	CommandDispatcher dispatecher;

	std::string request = "fsa A1 a(a+b)*b";
	cout << dispatecher.dispatch(request) << endl;

	request = "fsa A2 b(a+b)*a";
	cout << dispatecher.dispatch(request) << endl;

	request = "A3 = union A1 A2";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts A3 aabbab";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts A3 baabbab";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts A3 aaabbaa";
	cout << dispatecher.dispatch(request) << endl;

	request = "min A3";
	cout << dispatecher.dispatch(request) << endl;

	request = "print A3";
	cout << dispatecher.dispatch(request) << endl;
}

void cfgTest()
{
	CommandDispatcher dispatecher;

	std::string request = "cfg test S->aSd|A A->bAcc|$";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts test aaabbccccddd";
	cout << dispatecher.dispatch(request) << endl;

	request = "accepts test aaaaabbcccccddd";
	cout << dispatecher.dispatch(request) << endl;
}
void runAFL()
{
	CommandDispatcher dispatcher;
	while (1)
	{
		cout << ">";
		std::string request;
		std::getline(cin, request);
		std::string response = dispatcher.dispatch(request);
		cout << response << endl;

		std::string envOutput1 = dispatcher.env.toStringFSA();

		std::string envOutput2 = dispatcher.env.toStringNPDA();
	}
}
int main() 
{
	//npdaTest();
	//fsaTest();
	cfgTest();
	runAFL();
}
 
